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

class A
{
private:
    int te;
public:
    A():te(8){}
    void cll(const A& test)
    {
        if(te == test.te)
            cout << "hello world" << endl;
    }
};

int main()
{
    string te(5,'d');
    cout << te;
    return 0;
}
