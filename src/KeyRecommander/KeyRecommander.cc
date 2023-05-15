#include "../../include/KeyRecommander/KeyRecommander.h"


extern __thread const char * name;


KeyRecommander::KeyRecommander(const string& word)
: _queryWord(word)
{

}

void KeyRecommander::queryIndexTable()//从索引文件找出查询词的近似词
{
    Dictionary* dictionary = Dictionary::createInstance();//获取Dictionary
    set<int> line_no;// 每个字符对应的行号
    vector<string> queryWord = read_utf8_onebyone(_queryWord); // 将待查询的字符串每一个元素存入数组

    // std::cout<<_queryWord<<std::endl;
    for(auto elem:queryWord)
    {
        if((dictionary->getIndexTable().find(elem)) != (dictionary->getIndexTable().end()))
        {
            set<int> temp_line_no = dictionary->getIndexTable().at(elem);// 找出每一个元素对应的行号set集合

            line_no.insert(temp_line_no.begin(), temp_line_no.end());// 并将集合插入到集合line_no中
        }
    }
    // std::cout<<line_no.size()<<std::endl;
    /*
        h: 1 200 300 400
        e: 1 200 400 500
        l: 1 200 500 700
        o: 1 200 700
    */ 
    int i = 0;// 从0行开始遍历
    // 遍历词典（词典中存储的是单词+词频）
    // 把行号set集合中存在的行号所对应的单词和词频存入_similarWord
    for(auto elem: dictionary->getDict()) 
    {
        if(line_no.count(i))
        {
            _similarWord.push_back(make_pair(elem.first, elem.second));
        }
        i++;
    }
    // std::cout<<_similarWord.size()<<std::endl;
}
//将近似词进行计算（最小编辑距离，词频，候选词）结果放入优先队列
void KeyRecommander::statistic()
{
    struct CandidateResult myresult;
    for(auto elem: _similarWord)
    {
        bzero(&myresult, sizeof(myresult));//清空myresult结构体
        myresult._dist = editDistance(_queryWord, elem.first); //计算出最小编辑距离
        myresult._freq = elem.second; //获取单词对应的词频
        myresult._word = elem.first; //把词语存入
        _que.push(myresult); // 将结构体放入优先队列
    }
}

void KeyRecommander::product_similar_finally()//产生优先级最高的三个近似词
{
    int i = 0;
    while(!_que.empty() && i++ < 3)
    {
        _similar_finally.push_back(_que.top()._word);
        _que.pop();
    }
}

void KeyRecommander::printSimilar()
{
    std::cout<<_similar_finally.size()<<std::endl;
    for(auto x:_similar_finally)
    {   
        std::cout << x << std::endl;
    }
}
// 将优先级最高的三个近似词以json类型返回
// json KeyRecommander::get_serialize()
// {
//     json j;
//     if(CacheManager::getCachemanager()->getKeyWrodCache(atoi(name)).get(_queryWord, j))
//     {
//         return j;
//     }
//     else 
//     {
//         executeQuery();
//         j = ProtocolParser::vector2json(_similar_finally);
//         CacheManager::getCachemanager()->getKeyWordCache(atoi(name)).put(_queryWord, j);
//         return j;
//     }
// }

vector<string> KeyRecommander::read_utf8_onebyone(string input)
{
    vector<string> words;
    int len = input.length();
    int i = 0;

    while(i < len)
    {
        assert((input[i] & 0xF8) <= 0xF0);
        int next = 1;
        if((input[i] & 0x80) == 0x00)
        {}
        else if((input[i] & 0xE0) == 0xC0)
        {
            next = 2;
        }
        else if((input[i] & 0xF0) == 0xE0)
        {
            next = 3;
        }
        else if((input[i] & 0xF8) == 0xF0)
        {
            next = 4;
        }
        words.push_back(input.substr(i, next));
        i += next;
    }
    return words;
}
void KeyRecommander::executeQuery() //开始执行查询
{
    // std::cout<<"executeQuery() "<<std::endl;
    queryIndexTable();// 先生成存放所有近似词的vector数组
    // std::cout<<"queryIndexTable() finish! "<<std::endl;
    statistic();// 在将每一个近似词对应的MyResult结构体存入优先队列
    // std::cout<<"statistic() finish! "<<std::endl;
    product_similar_finally();
    // std::cout<<"product_similar_finally() finish! "<<std::endl;
    printSimilar();
}
