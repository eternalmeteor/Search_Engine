#include "../../include/TcpServer/SocketIO.h"
#include <unistd.h>

SocketIO::SocketIO(int fd)
: _fd(fd)
{}


int SocketIO::recvn(char *buf, int len)
{
    char *pstr = buf;
    // 这里的left代表最要读取的字节数
    int left = len; // 还剩left个字节未读
    while(left > 0)
    {
        int ret = ::read(_fd, pstr, left);
        if(-1 == ret && errno == EINTR) // 发生中断
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("read");
            return len - left;
        }
        else if(0 == ret)
        {
            break;
        }
        else 
        {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;//返回真正读取的字节数
}


// 用recv每次读取一行数据
int SocketIO::readLine(char *buf, int len)
{
    char *pstr = buf;
    int total = 0; // 共计读取了total个字节
    while(len - total > 0)
    {
        // 这里使用了recv + MSG_PEEK,不会讲缓冲区中的数据进行清空
        int ret = ::recv(_fd, pstr, len - total, MSG_PEEK);
        if(-1 == ret && errno == EINTR) // 发生了中断
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("readLine");
            return total;
        }
        else if(0 == ret) //读完了
        {
            break;
        }
        else
        {
            // 这里执行真正的读操作
            for(int idx = 0; idx < ret; ++idx)
            {
                if(pstr[idx] == '\n')
                {
                    int sz = idx + 1;
                    recvn(pstr, sz);
                    total += sz;
                    pstr[sz] = '\0'; // '\n'后面加上结束符0

                    return total;
                }
            }
            recvn(pstr, ret);// 从内核态拷贝到用户态，这里才会发生真正的读操作
            total += ret;
            pstr += ret;
        }
    }
    *pstr = '\0';

    return total;
}
int SocketIO::sendn(const char *buff, int len)
{
    const char *pbuf = buff;

    int left = len;
    while(left > 0)
    {
        // 注意这里使用了send发送数据，主要是为了避免对端关闭连接，
        // 继续使用write发送数据将产生SIGPIPE信号，这可能会导致程序终止。
        int ret = send(_fd, pbuf, left, MSG_NOSIGNAL);
        if(ret == -1 && errno == EINTR)
        {
            continue;
        } else if(ret == -1)
        {
            perror("send");
            return len - left;
        } else if(ret == 0)
        {
            return len - left;
        } else {
            left -= ret;
            pbuf += ret;
        }
    }
    return len - left;
}
int SocketIO::readjson(char *buff, int maxlen)
{
    char *pbuf = buff;
    int left = maxlen - 1;
    int total = 0;
    while(left > 0) 
    {
        int ret = recv(_fd, pbuf, left, MSG_PEEK);
        if(ret == -1 && errno == EINTR)
        {
            continue;
        } else if(ret == -1)
        {
            perror("recv");
            return maxlen - left - 1;
        } else if(ret == 0)
        {
            return maxlen - left;
        } else {
            for(int idx = 0; idx < ret; idx++) {
                if(pbuf[idx] == '}')
                {
                    int sz = idx + 1;
                    recvn(pbuf, sz);
                    return sz + total;
                }
            }
            recvn(pbuf, ret);
            left -= ret;
            pbuf += ret;
            total += ret;
        }
    }
    buff[maxlen - 1] = '\0';
    return maxlen - 1;
}