#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

/*网络地址类，负责所有的地址相关的操作，获取ip地址，获取端口号，strcut sockaddr_in
*/

using std::string;

class InetAddress
{
private:
    // IPV4
    // #include <netinet/in.h> 
    // struct sockaddr_in { 
    //     sa_family_t sin_family; /* __SOCKADDR_COMMON(sin_) */ 
    //     in_port_t sin_port; /* Port number. */ 
    //     struct in_addr sin_addr; /* Internet address. */ 
    //     /* Pad to size of `struct sockaddr'. */ 
    //     unsigned char sin_zero[sizeof (struct sockaddr) - __SOCKADDR_COMMON_SIZE - sizeof (in_port_t) - sizeof (struct in_addr)]; 
    // };
    struct sockaddr_in _addr;
public:
    InetAddress(const string &ip, unsigned short port)
    {
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        /* Convert Internet host address from numbers-and-dots notation in CP
   into binary data in network byte order.  */
        _addr.sin_addr.s_addr = inet_addr(ip.c_str());
        //h --> host n --> net l --> 32bit s --> 16bit
        // 大小端转换
        _addr.sin_port = htons(port);
    }
    InetAddress(const struct sockaddr_in &addr) 
    : _addr(addr) 
    {}

    string ip() const
    {
        //将32位网络字节序转换成点分十进制
        return string(inet_nota(_addr.sin_addr));
    }

    unsigned short port() const
    {
        // h --> host n --> net l --> 32bit s --> 16bit
        // 大小端转换
        return ntohs(_addr.sin_port);
    }

    const struct sockaddr_in *getInetAddressPtr()
    {
        return &_addr;
    }
    


};




#endif