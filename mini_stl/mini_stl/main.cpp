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

int main()
{
    char * c = mini_stl::allocator<char, mini_stl::alloc>::allocate(1);
    mini_stl::construct(c,'a');
    cout << "a" << c  << "a" << endl;
    mini_stl::destroy(c);
    cout << "a" << c  << "a" << endl;
    return 0;
}
