//
//  main.cpp
//  UtilityBox
//
//  Created by Diane on 10/3/13.
//  Copyright (c) 2013 Diane liudi. All rights reserved.
//

#include <iostream>
#include "CData.h"
void Printf(CData<int> &a)
{
    size_t i=0;
    while (i<a.GetBufSize()) {
        cout << a[i++] << " ";
    }
    cout << endl;
}
int main(int argc, const char * argv[])
{
    CData<int> a(5);
    CData<int> b(13);
    int d[123] = {1,2,3,4,5,6,8,9,0};
    
    cout << sizeof(d)/sizeof(int) << endl;
    
    a.append(d, 9);
    Printf(a);
    cout << endl;
    a.append(d, 9);a.append(d, 9);a.append(d, 9);a.append(d, 9);
    Printf(a);
    cout << a.GetBufSize() << endl;
    
    CData<int> f(100);
    f.append(a);
    Printf(f);
    return 0;
}

