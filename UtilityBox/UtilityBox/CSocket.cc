//
//  CSocket.cc
//  UtilityBox
//
//  Created by Diane on 10/6/13.
//  Copyright (c) 2013 Diane liudi. All rights reserved.
//

#include "CSocket.h"
#include <pthread.h>

bool CBaseSocket::Init()
{
    return true;
}

bool CBaseSocket::Clean()
{
    return true;
}

char* CBaseSocket::DnsParse(const char *szHost)
{
    static char szIP[32] = {0};
    hostent *p = gethostbyname(szHost);
    if (p == NULL) {
        return NULL;
    }
    sprintf(szIP,"%u.%u.%u.%u",
            p->h_addr_list[0][0],
            p->h_addr_list[0][1],
            p->h_addr_list[0][2],
            p->h_addr_list[0][3]);
    return szIP;
}

CBaseSocket::CBaseSocket(SOCKET socket /* = -1*/)
{
    _socket = socket;
    _iAF = AF_UNSPEC;
}

CBaseSocket::~CBaseSocket()
{
    if (_socket != INVALID_SOCKET) {
        Close();
    }
}

CBaseSocket& CBaseSocket::operator=(SOCKET socket)
{
    if (Close()) {  //altered
         _socket = socket;
    }
    return *this;
}

SOCKET CBaseSocket::Create(int iAF, int iType, int iProtocol)
{
    _socket = socket(iAF, iType, iProtocol);
    _iAF = iAF;
    return _socket;
}
bool CBaseSocket::Close()
{
    if (shutdown(_socket, SHUT_RDWR)) {//altered
        _socket = INVALID_SOCKET;
        return true;
    }
    else
    {
        return false;
    }
}

SOCKET CBaseSocket::GetSocket() const
{
    return _socket;
}

int CBaseSocket::GetSocketError() const
{
    return errno;
}

ushort CBaseSocket::Bind(ULONG ulHost, USHORT usPort, bool bTrav)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    
    addr.sin_family = _iAF;
    addr.sin_port = htons(usPort);
    addr.sin_addr.s_addr = htons(ulHost);//problem?
    
    if (!bTrav) {
        return (bind(_socket, (const struct sockaddr*)&addr, sizeof(addr)) < 0)? 0:usPort;
    }
    else
    {
        USHORT usPortTrav = usPort;
        while (true)
        {
            if (bind(_socket, (const sockaddr*)&addr, sizeof(addr)) < 0)
            {
                addr.sin_port = htons(++usPortTrav);
                if (usPortTrav == 65535) {
                    return 0;
                }
                
            }
            else
                return usPortTrav;
        }
    }
}

USHORT CBaseSocket::Bind(USHORT usPort, bool bTrav)
{
    return Bind(INADDR_ANY, usPort, bTrav);
}

bool CBaseSocket::SetSockOpt(int iLevel, int iOptname, const char *iOptval, int iOptlen)
{
    if (setsockopt(_socket, iLevel, iOptname, iOptval, iOptlen) == 0) {
        return true;
    }
    else
        return false;
}

char* CBaseSocket::NtoA(ULONG ulHost)//problem?
{
    in_addr in;
    in.s_addr = htons(ulHost);
    return inet_ntoa(in);
}

ULONG CBaseSocket::AtoN(const char *szHost)
{
    return inet_addr(szHost);
}

///////////////////////////////////////////////////////////////////////
CTCPSocket::CTCPSocket(SOCKET socket /* = INVALID_SOCKET*/)
{
    
}

CTCPSocket::~CTCPSocket()
{
    
}
SOCKET CTCPSocket::Create(int iAF/* = AF_INET*/)
{
    return baseSock.Create(iAF, SOCK_STREAM, IPPROTO_TCF);
}

bool CTCPSocket::Close()
{
    return baseSock.Close();
}

bool CTCPSocket::Close(SOCKET accept)
{
    return true;//problem?
}

bool CTCPSocket::Shutdown(SOCKET accept, int iHow)
{
    if (shutdown(accept, iHow) == 0) {
        return true;
    }
    else
        return false;
}

bool CTCPSocket::Connect(const char* szHost, USHORT usPort)
{
    return Connect(inet_addr(szHost), usPort);
}

bool CTCPSocket::Connect(ULONG ulHost, USHORT usPort)
{
    sockaddr_in destAddr;
    bzero(&destAddr, sizeof(destAddr));
    
    destAddr.sin_family = _iAF;
    destAddr.sin_port = htons(usPort);
    destAddr.sin_addr.s_addr = htons(ulHost);//problem?
    if (connect(baseSock._socket, (const sockaddr*)&destAddr, sizeof(destAddr)) == 0)
    {
        return true;
    }
    else
        return false;
}

SOCKET CTCPSocket::Accept()
{
    return accept(_socket, NULL, NULL);
}

SOCKET CTCPSocket::Accept(ULONG &ulHost, USHORT &usPort)
{
    sockaddr_in destAddr;
    bzero(&destAddr, sizeof(destAddr));
    
    int iAddrLen = sizeof(destAddr);
    SOCKET sock = accept(_socket, (sockaddr *)&destAddr, &destAddr);
    ulHost = destAddr.sin_addr.s_addr;//problem?
    usPort = ntohs(destAddr.sin_port);
    return sock;
}

int CTCPSocket::Send(const void *pBuf, int iLen)
{
    return send(_socket, (const char*)pBuf, iLen, 0);
}

int CTCPSocket::Send(SOCKET accept, const void *pBuf, int iLen)
{
    return send(accept, (const char*)pBuf, iLen, 0);
}

int CTCPSocket::Recv(void *pBuf, int iLen)
{
    return recv(_socket, (char*)pBuf, iLen, 0);
}

int CTCPSocket::Recv(SOCKET accept,  void *pBuf, int iLen)
{
    return recv(accept, (char*)pBuf, iLen, 0);
}

CTCPServer::CTCPServer()
{
    _iMaxWorkThread = -1;
    _iWorkThreadCounter = -1;
    CBaseSocket<CTCPServer>::Init();//?
}

CTCPServer::~CTCPServer()
{
    if (_TCP.GetSocket() != INVALID_SOCKET) {
        _TCP.Close();
    }
    CBaseSocket<CTCPServer>::Clean();
}

bool CTCPServer::StartServer(int iMaxNumOfWorkThread, USHORT usPort)
{
    if(_TCP.GetSocket() == INVALID_SOCKET)
    {
        if (_TCP.Create() == INVALID_SOCKET) {
            return false;
        }
    }
    else
        return False;//?
    
    int iBufLen = 64*1024;//设置缓冲区长度
    if (!_TCP.SetSockOpt(SOL_SOCKET, SO_RCVBUF, (const char*)&iBufLen, sizeof(iBufLen))) {
        return false;
    }
    if (!_TCP.SetSockOpt(SOL_SOCKET, SO_SNDBUF, (const char*)&iBufLen, sizeof(iBufLen))) {
        return false;
    }
    
    int iMilliScnds = 1000*1000;
    if (!_TCP.SetSockOpt(SOL_SOCKET, SO_RCVTIMEO, (const char*)&iMilliScnds, sizeof(iBufLen))) {
        return false;
    }
    if (_TCP.Bind(usPort) == 0) {
        return false;
    }
    if (!_TCP.Listen()) {
        return false;
    }
    
    _iMaxWorkThread = iMaxNumOfWorkThread;
    pthread_t id;
    int ret = pthread_create(&id, NULL, CTCPServer::AcceptThread, this);
    pthread_join(id, NULL);
    return true;
}

void* CTCPServer::AcceptThread(void* pParam)
{
    CTCPServer *pThis = (CTCPServer*)pParam;
    while (true) {
        SOCKET accept = pThis->_TCP.Accept();
        if (accept == INVALID_SOCKET) {
            continue;
        }
        else
        {
            if (pThis->_iWorkThreadCounter >= pThis->_iMaxWorkThread)
            {
                continue;
            }
            pthread_t id = 0;//?
            pthread_create(&id, NULL, WorkThread, (void*)accept);//?
        }
    }
    pthread_exit(NULL);//?
}

