#include "../../include/ThreadPool/Thread.h"

// 这里不懂__thread，进行学习
// __thread定义的变量实现了线程局部存储，
// 这里不使用string而使用const char* 是因为string不是pod类型
// 线程局部存储（TLS--Thread Local Storage）
// TLS 是一种机制，通过这一机制分配的变量，每个当前线程有一个该变量的实例。
// extern __thread const char *name; // 这里一般在头文件中进行声明
__thread const char *name = "thread__name";

Thread::Thread(ThreadCallback &&cb, const string &name)
: _thid(0)
, _isRunning(true)
, _cb(std::move(cb))
, _name(name) 
{}

Thread:: ~Thread() 
{
    if(_isRunning)
    {
        pthread_detach(_thid);
        _isRunning = false;
    }
}



void Thread::start()
{
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret) {
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}
// 调用此函数相当于主线程主动的去回收子线程的资源
void Thread::join()
{
    if(_isRunning)
    {
        /*
        功能：和一个已经终止的线程进行连接。回收子线程的资源，
        这个函数是阻塞函数，调用一次只能回收一个子线程，一般在主线程中使用
        */
       /*
       `int pthread_join(pthread_t thread, void **retval);`

        - 通过`man 3 pthread_join`查看帮助
        - 功能：和一个已经终止的线程进行连接。回收子线程的资源，这个函数是阻塞函数，调用一次只能回收一个子线程，一般在主线程中使用
        - 参数
            - `thread`：需要回收的子线程的ID
            - `retval`：接收子线程退出时的返回值
        - 返回值
            - 成功：0
            - 失败：返回错误号。这个错误号和之前`errno`不太一样。获取错误号的信息：  `char * strerror(int errnum);`
        */
        pthread_join(_thid, nullptr);
        _isRunning = false;
    }
}
    
void * Thread::threadFunc(void *arg) 
{
    Thread *pth = static_cast<Thread*>(arg);
    name = pth->_name.c_str(); // 从string转换为const char*
    if(pth) {
        pth->_cb();// 回调函数--线程需要真正运行的任务
    }
    return nullptr;
}



