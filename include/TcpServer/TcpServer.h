#ifndef __TCP_SERVER_H
#define __TCP_SERVER_H


class TcpServer
{
private:
    Acceptor _acceptor;
    EventLoop _loop;
public:
    TcpServer(const string & ip, unsigned short port);
    void setAllCallbacks(TcpConnectionCallback &&,
                    TcpConnectionCallback &&,
                    TcpConnectionCallback&&);
    void start();
    void stop();
    EventLoop *getLoop();
};



#endif