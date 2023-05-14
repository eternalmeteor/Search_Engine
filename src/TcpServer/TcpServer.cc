#include "../../include/TcpServer/TcpServer.h"


TcpServer::TcpServer(const string & ip, unsigned short port)
: _acceptor(ip, port)
, _loop(_acceptor)
{

}

void TcpServer::setAllCallback(TcpConnectionCallback && cb1,
                                TcpConnectionCallback && cb2,
                                TcpConnectionCallback && cb3)
{
    _loop.setAllCallback(std::move(cb1), std::move(cb2), std::move(cb3));
}

void TcpServer::start()
{
    _acceptor.ready();
    _loop.loop();
}

void TcpServer::stop()
{
    _loop.unloop();
}

EventLoop* TcpServer::getLoop()
{
    return &_loop;
}