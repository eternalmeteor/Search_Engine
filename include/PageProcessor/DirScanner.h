#ifndef _DIRSCANNER_H_
#define _DIRSCANNER_H_
#include "../myhead.h"

// 主要作用：遍历目录，获取文件
class DirScanner
{
public:
    void operator()(const std::string &dir);
    std::vector<std::string> &getFiles();
    void traverse(const std::string &dir);
private:
    std::vector<std::string> _files;
};



#endif