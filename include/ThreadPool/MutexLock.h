#ifndef _MUTEXLOCK_H__
#define _MUTEXLOCK_H__

#include "NoCopyable.h"
#include <pthread.h>

class MutexLock
: NoCopyable
{
public:
    MutexLock();
    ~MutexLock();
    void lock(); // 锁函数
    void unlock();

    pthread_mutex_t *getMutexLockPtr()
    {
        return &_mutex;
    }
private:
    pthread_mutex_t _mutex; // 互斥变量

};

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock &mutex)
    : _mutex(mutex)
    {
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        _mutex.unlock();
    }
private:
    MutexLock &_mutex;
};



#endif 
