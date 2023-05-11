#include "../../include/TcpServer/Acceptor.h"


class Acceptor
{
private:
    Socket _sock;
    InetAddress _servAddr;
public:
Acceptor::Acceptor(const string &id, unsigned short port)
: _sock()
, _servAddr(id, port)
{}


void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}
void Acceptor::setReuseAddr()
{
    int reuse = 1;
    /*
    `int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen); `
    - 功能：设置套接字的属性（不仅仅能设置端口复用），以下说明仅针对端口复用，其他详细内容可查看`slide/04Linux网络编程/02 socket通信/UNP（Unix网络编程）.pdf`第七章相关内容 
    - 参数
    - `sockfd`：要操作的文件描述符 
    - `level`：级别，`SOL_SOCKET` (端口复用的级别)
    - `optname`：选项的名称，使用`SO_REUSEADDR`或`SO_REUSEPORT`
    - `optval`：端口复用的值（整形） ，1表示可复用，0表示不可复用
    - `optlen`：optval参数的大小
    */
    int ret = setsockopt(_sock.fd(), SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    if(ret < 0)
    {
        perror("setsockopt");
        return;
    }
}

void Acceptor::setReusePort()
{
    int reuse = 1;
    int ret = setsockopt(_sock.fd(), SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));
    if(ret < 0)
    {
        perror("setsockopt");
        return;
    }
}

void Acceptor::bind()
{
    /*
    `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); `
    - 功能：绑定，将fd 和本地的IP和端口进行绑定 
    - 参数：
    - `sockfd`：通过socket函数得到的文件描述符 
    - `addr`：需要绑定的socket地址，这个地址封装了**本地的ip和端口号的信息**
    - `addrlen`：第二个参数结构体占的内存大小 
    - 返回值：成功：0，失败：-1
    */
    int ret = ::bind(_sock.fd(), (struct sockaddr *)_servAddr.getInetAddressPtr(),
    sizeof(struct sockaddr_in));
    if(ret < 0)
    {
        perror("bind");
        return;
    }
}

void Acceptor::listen()
{
    /*
    `int listen(int sockfd, int backlog);`
    - 功能：监听这个socket上的连接
    - 参数：
    - `sockfd`：通过socket()函数得到的文件描述符
    - `backlog`：未连接的和已经连接的和的最大值，可用`cat /proc/sys/net/core/somaxconn`查看Linux设置值，
    - 返回值：成功：0，失败：-1
    */
    int ret = ::listen(_sock.fd(), 1024);
    if(ret < 0)
    {
        perror("listen");
        return;
    }
}
int Acceptor::accept()
{
    /*
    `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); `
    - 功能：接收客户端连接，默认是一个阻塞的函数，阻塞等待客户端连接
    - 参数：
    - `sockfd` : 用于监听的文件描述符 
    - `addr` : 传出参数，记录了连接成功后**客户端的地址信息**（ip，port） 
    - `addrlen` : 指定第二个参数的对应的内存大小
    - 返回值：
    - 成功：用于通信的文件描述符 
    - 失败：-1*/
    int peerfd = ::accept(_sock.fd(), nullptr, nullptr);
    if(peerfd < 0)
    {
        perror("accept");
    }
    return peerfd;
}
