//
//  CSocket.cc
//  UtilityBox
//
//  Created by Diane on 10/6/13.
//  Copyright (c) 2013 Diane liudi. All rights reserved.
//

#include "CSocket.h"

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
    
}


