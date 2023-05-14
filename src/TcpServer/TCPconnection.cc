#include "../../include/TcpServer/TcpConnection.h"
#include "../../include/TcpServer/EventLoop.h"

TcpConnection::TcpConnection(int fd, EventLoop *loop)
: _sock(fd)
, _sockIO(fd)
, _localAddress(getLocalAddress())
, _peerAddress(getPeerAddress())
, _isShutdownWrite(false)
, _loop(loop)
{}

TcpConnection::string receive()
{
    char buff[65535] = {0};
    _sockIO.readLine(buff, sizeof(buff));

    return string(buff);
}

void TcpConnection::send(const string & msg)
{
    _sockIO.sendn(msg.c_str(), msg.size());
}

void TcpConnection::sendInLoop(const string &)
{
    // 通过eventfd通知ThreadPool进行相应的逻辑处理
    if(_loop) {
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}
void TcpConnection::shutdown()//关闭连接
{
    if(_isShutdownWrite == false)
    {
        _isShutdownWrite = true;
        _sock.shutdownWrite();
    }
}    
string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss << "tcp " << _localAddress.ip() << ":" <<_localAddress.port()
        << "--->" << _peerAddress.ip() << ":" << _peerAddress.port();
    return oss.str();
}
//注册三个事件的回调函数
void TcpConnection::setAllCallback(const TcpConnectionCallback &cb1,
                                const TcpConnectionCallback &cb2,
                                const TcpConnectionCallback &cb3)
{
    _onConnection = cb1;
    _onMessage = cb2;
    _onClose = cb3;
}
//三个回调函数的执行
void handleConnectionCallback()
{
    if(_onConnection)
    {
        _onConnection(shared_from_this());
    }
}
void TcpConnection::handleMessageCallback()
{
    if(_onMessage)
    {
        _onMessage(shared_from_this());
    }
}
void TcpConnection::handleCloseCallback()
{
    if(_onClose)
    {
        _onClose(shared_from_this());
    }
}
bool TcpConnection::isClosed() const
{
    int nret = -1;
    char buff[64] = {0};
    do {
        nret = recv(_sock.fd(), buff, 64, MSG_PEEK);
    } while(nret == -1 && errno == EINTR);

    return (nret == 0);
}

InetAddress TcpConnection::getLocalAddress()
{
    struct sockaddr_in localaddr;
    socklen_t len = sizeof(localaddr);
    int ret = getsockname(_sock.fd(), (struct sockaddr *)&localaddr, &len);
    if(ret < 0)
    {
        perror("getsockname");
    }
    return InetAddress(localaddr);
}
// 获取对端的网络ip地址
InetAddress TcpConnection::getPeerAddress()
{
    struct sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&peeraddr, &len);
    if(ret < 0)
    {
        perror("getpeername");
    }
    return InetAddress(peeraddr);
}