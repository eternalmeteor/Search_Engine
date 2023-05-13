#ifndef _SOCKETIO_H_
#define _SOCKETIO_H_
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

/*SocketIO类负责正常的读写操作*/
// 注意read与recv的区别，特别`是针对recv的第四个参数MSG_PEEK
class SocketIO
{
private:
    /* data */
    int _fd;
public:
    SocketIO(int);
    // ~SocketIO();
    int recvn(char *, int);
    int readLine(char *, int);
    int sendn(const char *, int);
    int readjson(char *, int);
};


#endif