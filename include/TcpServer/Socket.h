#ifndef _SOCKET_H_
#define _SOCKET_H_
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

/*套接字属于稀缺资源，最好能自动释放，N就需要使用RAII的思想;fd是不能
复制与赋值的*/
// 封装socket相关操作
class Socket
{
public:
    Socket();
    explicit Socket(int fd);
    int fd() const;
    void shutdownWrite();
    ~Socket();

private:
    int _sockfd;
};


#endif