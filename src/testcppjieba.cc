#include "../include/WordSegmentation.h"
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main() 
{
    string str = "结巴分词库的下载和应用";
    WordSegmentation wordSeg;
    vector<string> results = wordSeg(str);
    cout<<"分词结果如下："<<endl;
    for(auto it = results.begin();it != results.end();++it) {
        cout<<*it<<" ";
    }
    cout<<endl;
    // cout<<"hello"<<endl;
    return 0;
}