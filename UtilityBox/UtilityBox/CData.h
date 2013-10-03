//
//  CData.h
//  UtilityBox
//
//  Created by Diane on 10/3/13.
//  Copyright (c) 2013 Diane liudi. All rights reserved.
//
#include <assert.h>
#ifndef UtilityBox_CData_h
#define UtilityBox_CData_h
using namespace std;
typedef unsigned char byte;
typedef byte BYTE;

template<typename T = BYTE>
class CData{
	T *m_pBuf;
	std::ptrdiff_t m_BufLen;
public:
	explicit CData(std::ptrdiff_t len = 100)
    try:m_BufLen(len)
    {
        assert(len > 1);
        m_pBuf = new T[len]();
    }
    catch (std::bad_alloc){ m_pBuf = NULL;}
    
	~CData(){ delete []m_pBuf;}
    
	T* GetpBuf()const{return m_pBuf;}
	std::ptrdiff_t GetBufLen()const{ return m_BufLen;}
    
	T& operator[](std::ptrdiff_t i) const
	{
		assert(m_pBuf != NULL);
		assert(i>=0);
		assert(i<m_BufLen);
		return m_pBuf[i];
	} 
};

#endif
