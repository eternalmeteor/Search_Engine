#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Task.h"
#include "Thread.h"
#include "TaskQueue.h"


#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class ThreadPool
{
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    void start(); 
    void stop();

    void addTask(Task &&cb); // 往线程池中添加任务，本质上是往任务队列上添加任务。
    Task getTask(); 

private:
    void threadFunc();

private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit; // 这里的_isExit代表线程池是否存在
};




#endif