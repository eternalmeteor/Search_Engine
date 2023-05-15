#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "../myhead.h"
#include "../../include/SearchEngineServer/Configuration.h"


/*词典类*/
class Dictionary
{
private:
    class AutoRelease
    {
    public:
        ~AutoRelease()
        {
            if(_pdict)
                delete _pdict;
        }
    };
    Dictionary();
    ~Dictionary(){}
    vector<pair<string, int>> _dict; // 词典
    map<string, set<int>> _index; // 索引表
    static AutoRelease _ar;
    static Dictionary * _pdict;
    // static pthread_mutex_t _dmutex;
public:
    static Dictionary *createInstance();
    void init(const string &dictpath, const string & indexpath);
    vector<pair<string, int>>& getDict();
    map<string, set<int>> & getIndexTable();
};




#endif