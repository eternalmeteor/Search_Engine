#include "../../include/PageProcessor/WebPage.h"

std::ifstream WebPage::ripepageIfs;

WebPage::WebPage(int docid, const std::string &doctitle, const std::string &docurl, const std::string &doccontent)
    : docId(docid)
    , docTitle(doctitle)
    , docUrl(docurl)
      , docContent(doccontent)
{}

// 底下这个构造函数是执行网页查询的时候需要的
WebPage::WebPage(int docid, const std::unordered_map<int, std::pair<int, int>> &offsetLib, const std::vector<std::string> &queryWords, double weight)
    : docId(docid) 
{
    ripepageIfs.seekg(offsetLib.at(docid).first);
    int size = offsetLib.at(docid).second;
    char origDoc[size + 1];
    ::bzero(origDoc, sizeof(origDoc));
    ripepageIfs.read(origDoc, size);
    std::string doc(origDoc);

    size_t pos1 = doc.find("<url>") + 5,
    pos2 = doc.find("</url>", pos1),
    pos3 = doc.find("<title>", pos2 + 6) + 7,
    pos4 = doc.find("</title>", pos3),
    pos5 = doc.find("<content>", pos4 + 8) + 9;
    docUrl = doc.substr(pos1, pos2 - pos1);
    docTitle = doc.substr(pos3, pos4 - pos3);
    docContent = doc.substr(pos5, doc.find("<content>", pos5) - pos5);
    if(docContent.empty()) docContent = "----视频资料----";


    sortWeight = weight;
    // createSummary(queryWords);
}
// 查询的时候需要，所以先不着急写
void WebPage::createSummary(const std::vector<std::string> &queryWords)
{
}
