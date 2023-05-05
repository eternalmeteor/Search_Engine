#ifndef _WEBPAGE_H
#define _WEBPAGE_H
#include "../myhead.h"
#include "../SearchEngineServer/Configuration.h"

class WebPage
{ 
public:                                                                                                                                                                                                                                              
    WebPage() {}
    WebPage(int docid, const std::string &doctitle, const std::string &docurl, const std::string &doccontent);
    WebPage(int docid, const std::unordered_map<int, std::pair<int, int>> &offsetLib, const std::vector<std::string> &queryWords, double weight = 0);
    int docId;
    std::string docTitle;
    std::string docUrl;
    std::string docContent;
    std::string docSummary;
    std::map<std::string, int> wordsMap; // 保存每篇文档的所有词语和词频，不包括停用词
    double sortWeight;//对网页进行排序的依据,权重越大越靠前
private:
    void createSummary(const std::vector<std::string> &queryWords);
public:
    static std::ifstream ripepageIfs;
};



#endif