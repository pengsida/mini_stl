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
    mini_stl::string test;
    cin >> test;
    cout << test.at(1) << endl;
    cout << test << endl;
    cout << test.substr(1,3) << endl;
    test.append("fsdfsd");
    cout << test << endl;
    test.clear();
    cout << test << endl;
    test.append("afdl;fajdfal");
    cout << test << endl;
    test += "fajlf;ajdl";
    cout << test << endl;
    test.erase(test.rfind("faj"));
    cout << test << endl;
    cout << test.data() << endl;
    return 0;
}
