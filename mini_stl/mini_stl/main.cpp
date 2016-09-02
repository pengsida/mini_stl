//
//  main.cpp
//  mini_stl
//
//  Created by pengsida on 16/8/27.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#include <iostream>
#include "string_psd.h"
#include <string>
#include "allocator_psd.h"

using namespace std;

template<typename _CharT>
static size_t length(const _CharT* __s) {
    const _CharT __nullchar = _CharT();
    size_t __i;
    for (__i = 0; __s[__i] != __nullchar; ++__i)
    {}
    return __i;
}

int main()
{
    const char* str = "ad\0d";
    if(str[2] == char())
        cout << "hello world" << endl;
    return 0;
}
