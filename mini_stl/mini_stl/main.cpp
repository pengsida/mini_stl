//
//  main.cpp
//  mini_stl
//
//  Created by pengsida on 16/8/27.
//  Copyright © 2016年 pengsida. All rights reserved.
//

#include <iostream>
#include "list_psd.h"
#include <list>

using namespace std;


int main()
{
    list<int> te;
    te.push_back(400);
    te.push_back(10);
    list<int> tes;
    tes.push_back(1);
    tes.push_back(500);
    tes.push_back(3);
    tes.push_back(4);
//    te.insert(te.begin(), tes.begin(), tes.end());
//    te.resize(10,5);
//    te.splice(te.begin(), tes);
//    te.unique();
    te.merge(tes);
    for(list<int>::iterator itr = te.begin(); itr != te.end(); ++itr)
        cout << *itr << endl;
    cout << endl;
//    for(list<int>::iterator itr = tes.begin(); itr != tes.end(); ++itr)
//        cout << *itr << endl;
    return 0;
}
