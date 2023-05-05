#include "../../include/PageProcessor/DirScanner.h"
void DirScanner::operator()(const std::string &dir)
{
    traverse(dir);
}

std::vector<std::string> &DirScanner::getFiles()
{
    return _files;
}
/*const char *c_str();c_str()函数返回一个指向正规C字符串的指针常量, 内容与本string串相同。
这是为了与c语言兼容，在c语言中没有string类型，故必须通过string类对象的成员函数c_str()把string 对象转换成c中的字符串样式。
注意：一定要使用strcpy()函数 等来操作方法c_str()返回的指针。*/

void DirScanner::traverse(const std::string &dir) 
{
    /*
    DIR * opendir(const char * name);
    opendir()用来打开参数name 指定的目录, 并返回DIR*形态的目录流, 和open()类似, 接下来对目录的读取和搜索都要使用此返回值.
    */
    DIR *pdir = opendir(dir.c_str());
    if(nullptr == pdir)
    {
        perror("opendir");
    }

    struct dirent *pd;
    while((pd = readdir(pdir)))
    {
        if(strcmp(pd->d_name, ".") == 0 || strcmp(pd->d_name, "..") == 0)
        {
            continue;
        }
        _files.push_back(std::string(dir) + std::string("/") + std::string(pd->d_name));
    }
    closedir(pdir);
}