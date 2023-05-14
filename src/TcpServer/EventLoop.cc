#include "../../include/TcpServer/EventLoop.h"
#include <unistd.h>
#include <sys/eventfd.h>


EventLoop::EventLoop(Acceptor &acceptor)
: _epfd(createEpollfd())
, _evtfd(createEventfd())
, _acceptor(acceptor)
, _evtList(1024)
, _isLooping(false)
{
    addEpollReadFd(_acceptor.fd());
    addEpollReadFd(_evtfd);
}

int EventLoop::createEpollfd()
{
    int epfd = epoll_create(1);
    if(epfd < 0)
    {
        perror("epoll_create");
    }
    return epfd;
}

int EventLoop::createEventfd()
{
     /*eventfd 的使用

    特点：可以在进程或线程间进行通信。

    eventfd返回一个文件描述符，
    evtfd，A线程与B线程如果都可以看到evtfd。A线程取进行read操作，B线程进行write操作
    函数接口

    ```C++
    #include <sys/eventfd.h>
    int eventfd(unsigned int initval, int flags);
    initval:计数器的值，由内核进行维护。
    flag：早期版本（2.6.26）之前直接设置0,2.6.26版本之后设置为其他值。
    ```

    A进程对应是父进程，父进程进行read操作。会读evtfd文件描述符对应的数据，并且将该数据清空为0。

    B进程对应是子进程，子进程进行write操作，write可以将evtfd对应的内核维护的数据进行累加操作。*/
    
    // 这里使用eventfd的主要作用是为了使得IO线程和计算线程进行通信
    int evfd = eventfd(0,0);
    if(evfd < 0)
    {
        perror("eventfd");
    }
    return evfd;
}


void EventLoop::addEpollReadFd(int fd) //封装epoll_ctl
{
    struct epoll_event value;
    value.events = EPOLLIN;
    value.data.fd = fd;
    /*
    `int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);`

    - 功能：对epoll实例进行管理：添加文件描述符信息，删除信息，修改信息
    - 参数：
    - `epfd`：epoll实例对应的文件描述符
    - `op`：要进行什么操作
        - 添加：`EPOLL_CTL_ADD`
        - 删除：`EPOLL_CTL_DEL`
        - 修改：`EPOLL_CTL_MOD`
    - `fd`：要检测的文件描述符 
    - `event`：检测文件描述符什么事情，通过设置`epoll_event.events`，常见操作
        - 读事件：`EPOLLIN`
        - 写事件：`EPOLLOUT `
        - 错误事件：`EPOLLERR`
        - 设置边沿触发：`EPOLLET`（默认水平触发）
    - 返回值：成功0，失败-1
    */
    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &value);
    if(ret < 0) {
        perror("epoll_ctl");
        return;
    }
}
void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event value;
    value.events = EPOLLIN;
    value.data.fd = fd;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &value);
    if(ret < 0) {
        perror("epoll_ctl");
        return;
    }
}
EventLoop::~EventLoop()
{
    close(_epfd);
}

void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping) {
        waitEpollFd();
    }
}

void EventLoop::waitEpollFd()
{
    int nready = 0;
    do{
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 500000);
    } while(-1 == nready && errno == EINTR);
    if(-1 == nready)
    {
        perror("epoll_wait");
        return;
    } else if(nready == 0) {
        puts("epoll_wait timeout.");
    } else {
        if(nready ==(int)_evtList.size()) {
            _evtList.resize(2 * nready);
        }
        for(int idx = 0; idx < nready; idx++)
        {
            int fd = _evtList[idx].data.fd;
            if(fd == _acceptor.fd())
            {
                if(_evtList[idx].events & EPOLLIN)
                    handleNewConnection();
            } else if(fd == _evtfd)
            {
                handleRead(); //把eventfd的计数器缓存清空
                // cout<<"doPendingFunctors"
                // std::cout << "doPendingFunctors" << std::endl;
                #ifndef DEBUG
                std::cout << "doPendingFunctors" << std::endl;
                #endif
                // 唤醒所有回调函数，将所有的回调遍历执行完毕
                // 这里是执行计算线程（线程池）所需要的读写任务
                doPendingFunctors(); // 执行回调函数
            } else {
                //此时表明以前的连接有数据发送过来了
                if(_evtList[idx].events & EPOLLIN)
                    handleMessage(fd);
            }
        }
    }

}

/*处理新连接：添加读事件，设置连接对象*/
void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollReadFd(peerfd);
    TcpConnectionPtr conn(new TcpConnection(peerfd, this));
    conn->setAllCallback(_onConnection, _onMessage, _onClose);

    _conns.insert(std::make_pair(peerfd, conn));
    conn->handleConnectionCallback();
}

/*将eventfd的计数器清空*/
void EventLoop::handleRead()
{
    uint64_t howmany = 0;
    int ret = read(_evtfd, &howmany, sizeof(howmany));
    if(ret != sizeof(uint64_t))
    {
        perror("read");
    }
}
void EventLoop::doPendingFunctors()
{
    vector<Functor> tmp;
    {
        MutexLockGuard guard(_mutex);
        _pendingFunctors.swap(tmp);
    }
    for(auto &f:tmp)
    {
        f();
    }
}


void EventLoop::handleMessage(int fd)
{
    //查找TcpConnection对象
    // fd --> TcpConnection;
    auto iter = _conns.find(fd);
    if(iter != _conns.end()) {
        // 0.先判断该连接是否断开
        auto isClosed = iter->second->isClosed();
        if(!isClosed) {
            // 1.正常消息的处理
            iter->second->handleMessageCallback();
        } else {
            // cout<<"2.连接断开"<<endl;
            // 2.连接断开
            iter->second->handleCloseCallback();
            delEpollReadFd(fd);
            _conns.erase(iter);
        }
    }
}


void EventLoop::unloop()
{
    _isLooping = false;
}

void EventLoop::setAllCallback(TcpConnectionCallback &&cb1,
                                TcpConnectionCallback &&cb2,
                                TcpConnectionCallback &&cb3)
{
    _onConnection = std::move(cb1);
    _onMessage = std::move(cb2);
    _onClose = std::move(cb3);
}


void EventLoop::runInLoop(Functor && cb)
{
    {
        MutexLockGuard guard(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    wakeup();
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = write(_evtfd, &one, sizeof(one));
    if(ret != sizeof(uint64_t))
    {
        perror("write");
    }
}