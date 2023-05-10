#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include <queue>
#include "Condition.h"
#include "MutexLock.h"
#include "Task.h"

using std::queue;
using Elem = function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t queSize);
    ~TaskQueue();
    bool empty() const;
    bool full() const;
    void push(Elem &&value);
    Elem pop();
    
    // 将所有的等在_notEmpty上的线程唤醒
    void wakeup(); //唤醒_notEmpty条件变量

private:
    size_t _queSize; // 队列大小
    queue<Elem> _que;// 队列
    MutexLock _mutex; // 互斥锁对象
    Condition _notEmpty; // 缓冲区有数据的条件变量
    Condition _notFull; //缓冲区没有放满数据的条件变量
    bool _flag; // 标志位，用于退出pop函数
};




#endif