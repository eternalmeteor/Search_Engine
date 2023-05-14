#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <functional>
#include <memory>
#include <string>
#include <sstream>

using std::string;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
class EventLoop;
/*该类能创建完成，表明三次握手已经完成，后面的连接就是一个TCP连接，可以使用该连接进行通信，正常的数据收发*/
class TCPconnection
: public std::enable_shared_from_this<TcpConnection>
{
private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddress;//保存本端地址
    InetAddress _peerAddress;//保存对端地址
    bool _isShutdownWrite;
    EventLoop * _loop; // EventLoop本质上是epoll模型中while(1)的书写
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
public:
    TcpConnection(int, EventLoop *);
    string receive();
    void send(const string &);
    void sendInLoop(const string &);
    void shutdown();//关闭连接
    string toString() const;
    //注册三个事件的回调函数
    void setAllCallback(const TcpConnectionCallback &,
    const TcpConnectionCallback &, const TcpConnectionCallback &);
    //三个回调函数的执行
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
    bool isClosed() const;
private:
    InetAddress getLocalAddress();
    InetAddress getPeerAddress();
};




#endif