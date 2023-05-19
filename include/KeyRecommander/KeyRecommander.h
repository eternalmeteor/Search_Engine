#ifndef _KEYRECOMMANDER_H
#define _KEYRECOMMANDER_H

#include "CandidateResult.h"
#include "../myhead.h"
#include "../json.h"
#include "../TcpServer/TcpConnection.h"
#include "Dictionary.h"
#include "EditDistance.h"
#include "../ProtocolParser.h"

using json = nlohmann::json;

class ResultCompare
{
public:
    bool operator()(const CandidateResult &lhs, const CandidateResult &rhs)
    {
        if(lhs._dist == rhs._dist)
        {
            if(lhs._freq == rhs._freq)
            {
                if(lhs._word < rhs._word)
                {
                    // 此时表明距离相同，词频相同，按照字典序进行比较
                    return false;
                }
                else 
                {
                    return true;
                }
            }
            else 
            {
                // 此时表明距离相同，词频不同，词频大的在前
                if(lhs._freq < rhs._freq)
                {
                    // a的优先级高于b
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            if(lhs._dist < rhs._dist)
            {
                // a的优先级高于b，距离越小越靠前。
                return false;
            }
            else
            {
                return true;
            }
        }
    }
};

class KeyRecommander
{
private:
    string _queryWord;// 等待查询的单词
    priority_queue<CandidateResult,
    vector<CandidateResult>, ResultCompare> _que;//保存比较的结果
    vector<pair<string, int>> _similarWord;//存放近似词
    vector<string> _similar_finally;// 存放优先级最高的三个近似词
    TcpConnectionPtr _conn;
public:
    KeyRecommander(const string& word);
    void executeQuery(); //开始执行查询
    void queryIndexTable();//从索引文件找出查询词的近似词
    void statistic();//进行计算（最小编辑距离，词频，候选词）结果放入优先队列
    void product_similar_finally();//产生优先级最高的三个近似词
    json get_serialize();
private:
    vector <string> read_utf8_onebyone(string input);
    void printSimilar();
};

#endif
