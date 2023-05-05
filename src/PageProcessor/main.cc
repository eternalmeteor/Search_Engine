
#include "../../include/PageProcessor/PageLib.h"
#include <chrono>

using namespace std;

int main() 
{
    PageLib pl;
    pl.create();
    pl.store();
    auto t1 = std::chrono::high_resolution_clock::now();
    PageLibPreprocessor plp;

    return 0;
}