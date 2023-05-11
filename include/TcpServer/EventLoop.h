#ifndef _EVENTLOOP_H__
#define _EVENTLOOP_H__


#include "InetAddress.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "../ThreadPool/MutexLock.h"
#include "../myhead.h"
#include <sys/epoll.h>
#include <errno.h>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include "../ThreadPool/MutexLock.h"

using std::shared_ptr;
using std::map;
using std::vector;

using Functor = std::function<void()>;

/*EventLoop对应的线程：就是负责数据的收发，也就是基本IO操作，**IO线程***/
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
    EventLoop(Acceptor &);
    ~EventLoop();
    void loop();
    void runInLoop(Functor && cb);
    void unloop();
    void setAllCallback(TcpConnectionCallback &&cb1,
                        TcpConnectionCallback &&cb2,
                        TcpConnectionCallback &&cb3);
private:
    int createEpollfd(); // 创建epfd
    int createEventfd();

    void addEpollReadFd(int fd);
    void delEpollReadFd(int);

    void waitEpollFd();

    void handleNewConnection();
    void handleRead();
    void doPendingFunctors();
    void handleMessage(int);

    void wakeup();

};



#endif