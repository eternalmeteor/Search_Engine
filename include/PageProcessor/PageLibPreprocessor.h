#ifndef _PAGELIBPREPROCESSOR_H
#define _PAGELIBPREPROCESSOR_H

#include "../myhead.h"
#include "WebPage.h"
// #include "../DictProducer/SplitTool.h"
// #include "/home/ubuntu1804/Search_Engine/include/DictProducer/SplitTool.h"
#include "/home/ubuntu1804/Search_Engine/include/DictProducer/SplitTool.h"
class PageLibPreprocessor
{
public:
    PageLibPreprocessor();
    ~PageLibPreprocessor(){}
    void buildInvertIndexMap();
    void storeOnDisk();
private:
    std::vector<WebPage> _pageList;
    std::unordered_map<int, std::pair<int, int>> _offsetLib;
    std::unordered_map<std::string, std::map<int, double>> _invertIndexLib;
    SplitTool *_wordCutter;
};



#endif