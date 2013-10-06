//
//  CData.h
//  UtilityBox
//
//  Created by Diane on 10/4/13.
//  Copyright (c) 2013 Diane liudi. All rights reserved.
//

#ifndef UtilityBox_CData_h
#define UtilityBox_CData_h
#include <assert.h>
using namespace std;
typedef unsigned char byte;
typedef byte BYTE;

template<typename T = BYTE>
class CData{
private:
	T *m_pBuf;
    size_t m_BufSize;
	size_t m_BufLen;
    
public:
	explicit CData(size_t len = 100)
    try:m_pBuf(NULL),m_BufSize(0),m_BufLen(len)
    {
        assert(len > 1);
        m_pBuf = new T[len]();
    }
    catch (/*std::bad_alloc*/...)
    {
        delete []m_pBuf;
        m_pBuf = NULL;
    }
    
	~CData()
    {
        //如果构造函数抛出异常，需要继续调用析构函数，不进行判断会造成二次删除
        if (m_pBuf != NULL) {
            delete []m_pBuf;
            m_pBuf = NULL;
        }
    }
    
	T* GetpBuf()const{return m_pBuf;}
	size_t GetBufLen()const{ return m_BufLen;}
    size_t GetBufSize()const{return m_BufSize;}
    
	T& operator[](size_t i) const
	{
		assert(m_pBuf != NULL);
		assert(i>=0);
		assert(i<m_BufLen);
		return m_pBuf[i];
	}
    
    void append(T* data, size_t count);
    void append(CData<T> &elem);
    
private:
    CData(const CData<T>& elem);
    CData<T> operator=(const CData<T>& elem);
};

template <typename T>
void CData<T>::append(T* data, size_t count)
{
    if (m_BufSize + count > m_BufLen)
    {
        T *tmp = m_pBuf;
        try {
            m_pBuf = new T[m_BufLen+count]();
        } catch (std::bad_alloc) {
            m_pBuf = tmp;
            return;
        }
        m_BufLen += count;
        memcpy(m_pBuf, tmp, m_BufSize*sizeof(T));
        delete []tmp;
    }
    
    memcpy(m_pBuf+m_BufSize, data, count*sizeof(T));
    m_BufSize += count;
}

template <typename T>
void CData<T>::append(CData<T> &elem)
{
    this->append(elem.m_pBuf, elem.m_BufSize);
}
#endif
