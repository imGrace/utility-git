//
//  MakeSealed.h
//  UtilityBox
//
//  Created by Diane on 10/5/13.
//  Copyright (c) 2013 Diane liudi. All rights reserved.
//

#ifndef UtilityBox_MakeSealed_h
#define UtilityBox_MakeSealed_h
template<typename T>
class MakeSealed
{
    friend T;
    MakeSealed(){}
    ~MakeSealed(){}
};

//example
//virtual is required
class fun:public virtual MakeSealed<fun>
{
public:
    fun(){}
    ~fun(){}
    void sing(){std::cout << "lala" << std::endl;}
};

//error
//class fun1:public fun{
//public:
//    fun1(){}
//    ~fun1(){}
//};
#endif
