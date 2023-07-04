#include "../../include/ThreadPool/TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true)
{

}
TaskQueue::~TaskQueue()
{

}
bool TaskQueue::empty() const
{
    return _que.size() == 0;
}

bool TaskQueue::full() const
{
    return _que.size() == _queSize;
}

void TaskQueue::push(Elem &&value)
{
    {
        // 这里的lock与unlock必须要成对出现，否则会有死锁的困扰
        // 利用了RAII的思想
        MutexLockGuard autoLock(_mutex);
        // 如果使用if
        // 当full为true的时候，notFull会wait，
        // 假如notFull的wait被唤醒了，但是没有检测full()，很快又有任务添加进去了，
        // 此时队列又会是满的，造成que.push()失败
        while(full()) // 这里不能使用if，使用if会产生虚假唤醒
        {
            _notFull.wait();
        }
        _que.push(value);
    }
    _notEmpty.notify();
}

Elem TaskQueue::pop() // pop相当于是消费者线程
{
    Elem tmp;
    MutexLockGuard atuoLock(_mutex);
    
    // 这里加入_flag是为了防止任务执行得过快导致程序卡住
    // 可以看 void ThreadPool::threadFunc()函数得解析
    while(_flag && empty()) // 这里使用while同样是为了防止虚假唤醒
    {
        _notEmpty.wait(); // 到达wait之后线程睡眠
    }
    if(_flag) {
        tmp = _que.front();
        _que.pop();
        _notFull.notify();
        return tmp;
    } else {
        return nullptr;
    }
}
    
// 将所有的等在_notEmpty上的线程唤醒
void TaskQueue::wakeup() //唤醒_notEmpty条件变量
{
    _flag = false;
    _notEmpty.notifyAll();
}



