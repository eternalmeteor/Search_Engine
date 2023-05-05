#ifndef _DICTPRODUCER_H
#define _DICTPRODUCER_H
#include "/home/ubuntu1804/Search_Engine/include/DictProducer/SplitTool.h"
#include "/home/ubuntu1804/Search_Engine/include/myhead.h"

class DictProducer
{
public:
    DictProducer();// 构造函数
    DictProducer(SplitTool *cuttor);// 构造函数 专为中文处理
    void buildEnDict();// 创建英文字典
    void buildCnDict();// 创建中文字典
    void createIndex();
    void storeDict(const char *path);// 将词典写入文件
    void storeIndex(const char *path);// 将索引写入文件

private:
    vector<pair<string, int>> _dict;//词 词频->词典
    unordered_map<string,int> _dict_index;//词 词的下标
    map<string, set<int>> _index;//字 含有字的词的下标->词典索引
    SplitTool *_cuttor;
    map<string, string> &_config;
};


#endif