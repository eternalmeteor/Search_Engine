#ifndef __NOCOPTABLE_H__
#define __NOCOPTABLE_H__

class NoCopyable // 设置这个类的主要目的是让被继承的类不能够执行赋值和拷贝操作
{
public:
protected:
    NoCopyable(){}
    ~NoCopyable(){}
    NoCopyable(const NoCopyable &rhs) = delete;
    NoCopyable& operator=(const NoCopyable &rhs) = delete;
};



#endif