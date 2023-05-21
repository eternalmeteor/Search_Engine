#ifndef _TIMERTASK_H
#define _TIMERTASK_H

#include "../myhead.h"
// 含有纯虚函数的类称为抽象类，不能生成对象。
class TimerTask {
public:
    virtual ~TimerTask() {}
    virtual void process() = 0;
};

#endif // _TIMERTASK_H