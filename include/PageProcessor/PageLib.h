#ifndef _PAGELIB_H
#define _PAGELIB_H

#include "../SearchEngineServer/Configuration.h"
#include "../../lib/simhash/Simhasher.hpp"
struct HammingDistance_3
{
    bool operator()(const uint64_t &lhs, const uint64_t &rhs) const
    {   
        return simhash::Simhasher::isEqual(lhs, rhs);
    }   
};

class PageLib
{
public:
    PageLib();
    void create(); // 创建网页库
    void store(); // 存储网页库和位置偏移库
private:
    Configuration &_config;
    DirScanner _dirScanner; // 目录扫描对象的引用
    std::vector<std::string> _pages; //存放格式化之后的网页的容器
    simhash::Simhasher _simhasher;
    std::unordered_set<uint64_t, std::hash<uint64_t>, HammingDistance_3> _simhashset;
    std::map<int, std::pair<int, int>> _offsetLib; // 存放每篇文档在网页库的位置信息
};




#endif