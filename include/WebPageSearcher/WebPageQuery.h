#ifndef _WEBPAGEQUERY_H
#define _WEBPAGEQUERY_H

#include "../myhead.h"
#include "WebPage.h"


class WebPageQuery
{
private:
    unordered_map<int, pair<int, int>> _offsetlib;//偏移库
    unordered_map<string, map<int, double>> _invertIndexLib;//倒排索引库
    Configuration & _configs;
    SplitTools * _wordSegmentation;
public:
    WebPageQuery();
    ~WebPageQuery();
    void doQuery(const string &sought, vector<string> &retval);

private:
    void loadLibrary();
    vector<double> getQueryWordsWeightVector(const vector<string> & queryWords);
    bool executeQuery(const vector<string> & queryWords, vector<WebPage> & queryResult);
    string returnNoAnswer();
};

#endif