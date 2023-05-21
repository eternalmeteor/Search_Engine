#include "../../include/ThreadPool/ThreadPool.h"
#include "../../include/Cache/TimerManager.h"
#include "../../include/Cache/CacheTimerTask.h"
#include <unistd.h>


ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(queSize)
, _isExit(false)
{   
    _threads.reserve(threadNum);
    // 这里使用reserve函数直接进行内存的预分配，但是不进行元素的初始化，效率比resize高
}
ThreadPool::~ThreadPool()
{
    if(!_isExit) 
    {
        stop();
    }
}
// 这里本质上是建立了五个线程，其中设置name==0的线程有两个，其中一个执行start任务，主要用于更新缓存
void ThreadPool::start()
{
    for(size_t idx = 0; idx < _threadNum; idx++)
    {
        unique_ptr<Thread> up(new Thread(std::bind(&ThreadPool::threadFunc, this), std::to_string(idx)));
        _threads.push_back(std::move(up));
    }
    unique_ptr<Thread> up(new Thread(std::bind(&TimerManager::start, TimerManager::getTimerManager()), std::to_string(0)));
    _threads.push_back(std::move(up));
    for(auto &th : _threads)
    {
        th->start();
    }
}


void ThreadPool::addTask(Task &&task) // 往线程池中添加任务，本质上是往任务队列上添加任务。
{
    if(task) 
    {
        _taskQue.push(std::move(task));
    }
}   
Task ThreadPool::getTask()
{
    return _taskQue.pop();
} 

// 在线程池中封装的任务, 这个任务的实际执行者WorkThread
void ThreadPool::threadFunc()
{
    // 只要线程池没有退出，就可以一直获取任务，并且执行想要的task
    while(!_isExit)
    {
        // 如果getTask()执行得过快，或者过慢
        // 过慢：线程池的stop有可能会先执行，会将_isExit设置为True，线程任务执行不完
        // 但是getTask()执行得过快,来不及将isExit设置为true，
        // 一直卡在getTask()，也就是卡在Condition中的wait上面或者说test中的number没有输出完全，程序就终止了
        Task task = getTask();
        if(task) 
        {
            task();
        }
    }
}


void ThreadPool::stop()
{
    if(!_isExit)
    {
        // 下面的代码是为了防止getTask()函数执行得过慢
        // 只要任务队列中有数据，线程池中的工作线程就不能退出，让其sleep
        while(!_taskQue.empty())
        {
            sleep(1);
        }

        _isExit = true;
        // 防止getTask执行得过快，线程睡在wait上，以至于无法退出，程序一直卡在哪里
        // 所以需要使用wakeup()唤醒所有线程
        // 将所有等待在notEmpty上的工作线程全部唤醒
        _taskQue.wakeup();
        
        for(auto &th : _threads)
        {
            th->join();
        }


    }
}