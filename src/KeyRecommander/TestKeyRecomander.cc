#include "../../include/KeyRecommander/KeyRecommander.h"

#include <string>
using namespace std;

int main()
{
    string str = "国家";
    KeyRecommander KeyRecommander(str);
    // cout<<str<<endl;
    KeyRecommander.executeQuery();
    // cout<<"KeyRecommander.executeQuery();"<<endl;
    return 0;
}