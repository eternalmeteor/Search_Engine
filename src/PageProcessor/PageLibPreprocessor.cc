#include "../../include/PageProcessor/PageLibPreprocessor.h"
#include "../../include/SearchEngineServer/Configuration.h"
// #include "../../include/DictProducer/SplitTool.h"
#include "/home/ubuntu1804/Search_Engine/include/DictProducer/SplitTool.h"


PageLibPreprocessor::PageLibPreprocessor() : _wordCutter(Cppjieba::getSplitTool())
{
    // 初始化，主要目的是初始化类中的数据成员
    auto &config = *Configuration::getInstance();
    std::ifstream ifs(config["offset"]);
    int id, start, size;

    while(ifs >> id >> start >> size) 
    {
        _offsetLib.emplace(id, std::pair<int, int>(start, size));
    }
    ifs.close();
    ifs.open(config["ripepage"]);

    for(auto &offset : _offsetLib)
    {
        ifs.seekg(offset.second.first);
        int size = offset.second.second;
        char origDoc[size + 1]; 
        ::bzero(origDoc, sizeof(origDoc));
        ifs.read(origDoc, size);
        std::string doc(origDoc);

        size_t pos1 = doc.find("<docid>") + 7,
               pos2 = doc.find("</docid>", pos1),
               pos3 = doc.find("<url>", pos2 + 8) + 5,
               pos4 = doc.find("</url>", pos3),
               pos5 = doc.find("<title>", pos4 + 6) + 7,
               pos6 = doc.find("</title>", pos5),
               pos7 = doc.find("<content>", pos6 + 8) + 9;
        _pageList.emplace_back(std::stoi(doc.substr(pos1, pos2 - pos1)), doc.substr(pos5, pos6 - pos5), doc.substr(pos3, pos4 - pos3), doc.substr(pos7, doc.find("</content>") - pos7));
    }
    ifs.close();
}

// 建立倒排索引库并存储在std::unordered_map<std::string, std::map<int, double>> _invertIndexLib;
void PageLibPreprocessor::buildInvertIndexMap()
{
    std::map<std::string, int> DFmap;
    for(int idx = 0; idx < _pageList.size(); ++idx) 
    {
        std::vector<std::string> splictWords;
        _wordCutter->cut(_pageList[idx].docTitle + _pageList[idx].docContent, splictWords);
        for(auto &word : splictWords) 
        {
            if(!_pageList[idx].wordsMap[word]++) 
                ++DFmap[word]; // 某个词在所有文章中出现的次数
        }
    }
    // std::vector<WebPage> _pageList;
    // std::map<std::string, int> wordsMap; // 保存每篇文档的所有词语和词频，不包括停用词
    // _pageList[idx].wordsMap[word] 是TF, 某个词在文章中出现的次数
    std::unordered_map<int, double> dw; // 文章id + 文章权重w
    for(int idx = 0; idx < _pageList.size(); ++idx) 
    {
        int id = _pageList[idx].docId;
        for(auto &aPair : _pageList[idx].wordsMap) 
        {
            double w = aPair.second * log2(static_cast<double>(_pageList.size()) / (DFmap[aPair.first] + 1));
            _invertIndexLib[aPair.first].emplace(id, w); // 倒排索引库 单词/短语 + 文章id + 权重
            dw[id] += w*w;
        }
        dw[id] = sqrt(dw[id]);
    }
    // 归一化倒排索引库中的权重
    for(auto &wordMap : _invertIndexLib)
    {
        for(auto &p : wordMap.second)
        {
            p.second /= dw[p.first]; // p.first是文章id, dw[p.first] 是 sqrt(w1^2 + w2^2 + ...)
        }
    }
}

void PageLibPreprocessor::storeOnDisk()
{
    auto &config = *Configuration::getInstance();
    std::ofstream ofs(config["invertIndex"]);
    for(auto &line : _invertIndexLib)
    {
        ofs << line.first;
        for(auto &aPair : line.second) 
        {
            ofs<< " " << aPair.first << " " <<aPair.second;
        }
        ofs << "\n";
    }
    ofs.close();

}




