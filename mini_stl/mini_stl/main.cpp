//
//  main.cpp
//  mini_stl
//
//  Created by pengsida on 16/8/27.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#include <iostream>
#include "list_psd.h"
#include "vector_psd.h"
#include <list>

using namespace std;

int main()
{
    mini_stl::list<int> test;
    test.push_back(4);
    test.push_back(5);
    test.push_back(3);
    test.push_back(2);
    test.push_back(2);
    test.push_back(1);
    test.sort();
    test.insert(--test.end(), 2);
    
    mini_stl::list<int> tes;
    tes.push_back(1);
    tes.push_back(100);
    
    test.unique();
    
    for(mini_stl::list<int>::iterator itr = test.begin(); itr != test.end(); ++itr)
        cout << *itr << endl;
    
    cout << endl;
    
    for(mini_stl::list<int>::iterator itr = tes.begin(); itr != tes.end(); ++itr)
        cout << *itr << endl;
    return 0;
}
