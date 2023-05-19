#ifndef _SPLITTOOL_H_
#define _SPLITTOOL_H_
#include "/home/ubuntu1804/Search_Engine/include/myhead.h"
#include "/home/ubuntu1804/Search_Engine/include/cppjieba/Jieba.hpp"
class SplitTool
{
public:
    // 分词函数，纯虚函数，提供接口
    virtual vector<string> cut(const string &sentence, vector<string> &vec) = 0;
    // 虚析构函数
    virtual ~SplitTool(){}; 
};

class Cppjieba
: public SplitTool
{
public:
    static SplitTool *getSplitTool();
    vector<string> cut(const string &sentence, vector<string> &vec);
    static void destroy();
private:
    Cppjieba();
    ~Cppjieba(){}
    map<string, string> &_configs;
    cppjieba::Jieba _jieba;
    static SplitTool *splittool;
    static pthread_mutex_t _mutex;
};



#endif