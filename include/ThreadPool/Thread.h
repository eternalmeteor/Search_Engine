#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <functional>
#include "../myhead.h"
using std::function;
using std::bind;

using ThreadCallback = function<void()>;

extern __thread const char * name;
class Thread
{
public:
    // 这里为什么需要使用右值引用？
    // 为了减少深拷贝，这里最终调用的是函数指针，使用右值引用可以提高程序效率，减少内存分配次数
    Thread(ThreadCallback &&cb, const string &name);
    ~Thread();

    void start();
    void join();
    
private:
    // 线程入口函数
    static void *threadFunc(void *arg);


private:
    pthread_t _thid;
    string _name;
    bool _isRunning;
    // 要去实现的任务
    ThreadCallback _cb;
};



#endif