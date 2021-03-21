#include <iostream>
#include "singleton.h"
#include "MySingletonClass.h"

#include <cassert>
using namespace std;

int main()
{
    auto s1 = Singleton<MySingletonClass>::instance("hello");
    auto s  = Singleton<MySingletonClass>::instance();

    assert(s == s1);

    return 0;
}
