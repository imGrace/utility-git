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
#include <string.h>
#include "Singleton.h"

#ifndef UtilityBox_CSocket_h
#define UtilityBox_CSocket_h
typedef int SOCKET;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

const int INVALID_SOCKET = -1;

template <typename T>
class CBaseSocket
{
    friend T;
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

class CTCPSocket 
{
public:
    CTCPSocket(SOCKET socket = INVALID_SOCKET);
    ~CTCPSocket();
    SOCKET Create(int iAF = AF_INET);
    bool Close();
    bool Close(SOCKET accept);
    bool Shutdown(SOCKET accept, int iHow = SHUT_RDWR);
    
    bool Connect(const char* szHost, USHORT usPort);
    bool Connect(ULONG ulHost, USHORT usPort);
    
    bool Listen(int iBacklog = SOMAXCONN);
    SOCKET Accept();
    SOCKET Accept(ULONG &ulHost, USHORT &usPort);

    int Send(const void *pBuf, int iLen);
    int Send(SOCKET accept, const void *pBuf, int iLen);
    int Recv(void *pBuf, int iLen);
    int Recv(SOCKET accept, void *pBuf, int iLen);

    SOCKET GetSocket() const { return baseSock._socket;};
    bool SetSockOpt(int iLevel, int iOptname, const char* Optval, int iOptLen)
    {return baseSock.SetSockOpt(iLevel, iOptLen, Optval, iOptLen);}
    USHORT Bind(USHORT usPort, bool bTrav = false)
    {return baseSock.Bind(usPort);}
private:
    
    CBaseSocket<CTCPSocket> baseSock;
};

class CTCPServer : public singleton<CTCPSocket>
{
public:
    bool StartServer(int iMaxWorkThread, USHORT usPort);
    int Send(SOCKET accept, const void *pBuf, int iLen);
    int Recv(SOCKET accept, void *pBuf, int iLen);
    CTCPSocket &GetSocket();
private:
    CTCPServer();
    ~CTCPServer();
    static void AcceptThread(void *pParam);
    static void WorkThread(void *pParam);
    
    CTCPSocket _TCP;
    int _iMaxWorkThread;
    int _iWorkThreadCounter;
    void WorkCallBack(SOCKET accept);
    void CloseWorkCallBack(SOCKET accept);
};


#endif
