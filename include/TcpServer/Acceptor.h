#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__
#include "Socket.h"
#include "InetAddress.h"

/*Acceptor为接收器类，负责在连接器之前的基本操作,比如bind, listen, accept*/
class Acceptor
{
private:
    Socket _sock;
    InetAddress _servAddr;
public:
    Acceptor(const string &, unsigned short);
    void ready();
    int accept();
    int fd()
    {
        return _sock.fd();
    }
private:
    void setReuseAddr();
    void setReusePort();

    void bind();
    void listen();
};



#endif