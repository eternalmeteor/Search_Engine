#include "../../include/WebPageSearcher/WebPageQuery.h"

#include <string>
using namespace std;

int main()
{
    string str = "hello";
    WebPageQuery webPage;
    vector<string> vecStr;
    webPage.doQuery(str, vecStr);
    for(auto &x : vecStr)
    {
        cout << x << endl;
    }
    return 0;
}