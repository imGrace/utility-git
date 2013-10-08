//
//  CSocket.h
//  UtilityBox
//
//  Created by Diane on 10/6/13.
//  Copyright (c) 2013 Diane liudi. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#ifndef UtilityBox_CSocket_h
#define UtilityBox_CSocket_h
typedef int SOCKET;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

const int INVALID_SOCKET = -1;

class CBaseSocket
{
protected:
    CBaseSocket(SOCKET socket = -1);
    ~CBaseSocket();
    
    SOCKET _socket;
    int _iAF;
public:
    static bool Init();
    static bool Clean();
    static char *DnsParse(const char * szHost);
    
    CBaseSocket& operator= (SOCKET elem);
    SOCKET Create(int iAF, int iType, int iProtocol);
    bool Close();
    
    SOCKET GetSocket() const;
    int GetSocketError() const;
    
    USHORT Bind(ULONG ulHost, USHORT usPort, bool bTrav = false);
    USHORT Bind(USHORT usPort, bool bTrav = false);

    bool SetSockOpt(int iLevel, int iOptname, const char* Optval, int iOptLen);
    static char* NtoA(ULONG ulHost);
    static ULONG AtoN(const char* szHost);

};

#endif
