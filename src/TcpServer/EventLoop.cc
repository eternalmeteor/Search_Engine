#include "../../include/TcpServer/EventLoop.h"

class EventLoop
{
private:
    int _epfd; // epoll实例的文件描述符
    int _evtfd; // eventfd, 用于线程之间的通信
    Acceptor & _acceptor; // 接收器类
    map<int, shared_ptr<TcpConnection>> _conns; // 保存所有已经建立的连接
    vector<struct epoll_event> _evtList; // 存储触发事件的fd
    bool _isLooping; // 标记是否进行循环

    // 回调函数
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMe;
    TcpConnectionCallback _onConnection;

    vector<Functor> _pendingFunctors; // 需要延迟执行的回调函数
    MutexLock _mutex;
public:
EventLoop::EventLoop(Acceptor &)
: _epfd(createEpollfd())
{

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
    ~EventLoop();
    void loop();
    void runInLoop(Functor && cb);
    void unloop();
    void setAllCallback(TcpConnectionCallback &&cb1,
                        TcpConnectionCallback &&cb2,
                        TcpConnectionCallback &&cb3);
};