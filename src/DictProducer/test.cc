#include "/home/ubuntu1804/Search_Engine/include/DictProducer/DictProducer.h"
#include "/home/ubuntu1804/Search_Engine/include/DictProducer/SplitTool.h"
#include "/home/ubuntu1804/Search_Engine/include/SearchEngineServer/Configuration.h"


int main()
{
    SplitTool *stl = Cppjieba::getSplitTool();
    auto &configs = *Configuration::getInstance();
    DictProducer dp(stl);
    /*
        const char *c_str();
        c_str()函数返回一个指向正规C字符串的指针常量, 内容与本string串相同。
        这是为了与c语言兼容，在c语言中没有string类型，故必须通过string类对象的成员函数c_str()把string 对象转换成c中的字符串样式。
        注意：一定要使用strcpy()函数 等来操作方法c_str()返回的指针。
    */
    dp.storeDict(configs["xmldict_path"].c_str());
    dp.storeIndex(configs["index_path"].c_str());
    //dp.storeIndex("../data/dict/dictindex(1).dat");
    DictProducer dp1;
    dp1.storeDict(configs["engdict_path"].c_str());
    dp1.storeIndex(configs["en_index_path"].c_str());

    return 0;
}