//
//  main.cpp
//  mini_stl
//
//  Created by pengsida on 16/8/27.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#include <iostream>
#include "queue_psd.h"

using namespace std;

int main()
{
    mini_stl::queue<int> test;
    test.push(3);
    mini_stl::queue<int> tes;
    tes.push(3);
    if(test == tes) cout << "hello world" << endl;
    return 0;
}
