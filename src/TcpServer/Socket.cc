#include "../../include/TcpServer/Socket.h"


Socket::Socket()
: _sockfd(0)
{
    /*`int socket(int domain, int type, int protocol);`

    - 功能：创建一个套接字
    - 参数：
    - `domain`：协议族(常用如下)
        - `AF_INET` ：`ipv4`
        - `AF_INET`6 ：`ipv6`
        - `AF_UNIX`, `AF_LOCAL`：本地套接字通信（进程间通信） 
    - `type`：通信过程中使用的协议类型 
        - `SOCK_STREAM` : 流式协议
        - `SOCK_DGRAM` : 报式协议 
    - `protocol`：具体的一个协议，一般写0，用于指定type参数的默认协议类型
        - `SOCK_STREAM` : 流式协议默认使用 TCP 
        - `SOCK_DGRAM` : 报式协议默认使用 UDP 
    - 返回值
    - 成功：返回文件描述符，操作的就是内核缓冲区
    - 失败：-1 
    */
    /*
    int socket(int domain, int type, int protocol);
    //domain AF_INET --> IPv4 AF_INET6 --> IPv6
    //type SOCK_STREAM --> TCP SOCK_DGRAM --> UDP
    //protocol IPPROTO_TCP --> TCP IPPROTO_UDP -->UDP
    */
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(_sockfd < 0) 
    {
        perror("socket");
    }
}
Socket::Socket(int fd)
: _sockfd(fd)
{}
    
Socket::~Socket()
{
    close(_sockfd);
}
int Socket::fd() const
{
    return _sockfd;
}
void Socket::shutdownWrite()
{
    /*
    - `半关闭`：当 TCP 连接中 A 向 B 发送 FIN 请求关闭，另一端 B 回应 ACK 之后（A 端进入 FIN_WAIT_2状态），并没有立即发送 FIN 给 A，A 方处于半连接状态（半开关），此时 **A 可以接收 B 发送的数据，但是 A 已经不能再向 B 发送数据**
    - API 来控制实现半连接状态的方法：` shutdown函数`
    - `int shutdown(int sockfd, int how); `
        - 功能：实现半连接状态
        - 参数
        - `sockfd`：需要关闭的socket的描述符 
        - `how`：允许为shutdown操作选择以下几种方式
            - `SHUT_RD(0)`：关闭sockfd上的读功能，此选项将不允许sockfd进行读操作，该套接字不再接收数据，任何当前在套接字接受缓冲区的数据将被无声的丢弃掉
            - `SHUT_WR(1)`：关闭sockfd的写功能，此选项将不允许sockfd进行写操作。进程不能在对此套接字发 出写操作
            - `SHUT_RDWR(2)`：关闭sockfd的读写功能。相当于调用shutdown两次：首先调用`SHUT_RD`,然后调用 `SHUT_WR`

    ### shutdown与close

    - 使用 `close` 中止一个连接，但它只是**减少描述符的引用计数，并不直接关闭连接**，只有当描述符的引用计数为 0 时才关闭连接
    - `shutdown` 不考虑描述符的引用计数，**直接关闭描述符**。也可选择中止一个方向的连接，只中止读或只中止写
    - 如果有多个进程共享一个套接字，close 每被调用一次，计数减 1 ，直到计数为 0 时，也就是所用进程都调用了 close，套接字将被释放
    - **在多进程中如果一个进程调用了 `shutdown(sfd, SHUT_RDWR)` 后，其它的进程将无法进行通信**。但如果一个进程 `close(sfd)` 将不会影响到其它进程=>==难怪800那个项目调shutdown之后其他线程就不能用了==
    */
    shutdown(_sockfd, SHUT_WR);
}