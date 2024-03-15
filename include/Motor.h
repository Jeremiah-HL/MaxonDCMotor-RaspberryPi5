#pragma once
#include <iostream>
using namespace std;

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int BOOL;
typedef void* HANDLE;


class Motor {
protected:
    unsigned int errorCode;
    unsigned short m_NodeId;
    void* m_keyHandle;
    char* m_portName;
    int m_operationMode;

    int m_baudrate = 1000000;
    BOOL m_ifDefault = 0;

    static unsigned short deviceNum;
    static char deviceName[];
    static char protocolStackName[];
    static char interfaceName[];

    DWORD m_profileVelocity;
    DWORD m_profileAccerleration;
    DWORD m_profileDecerleration;

    virtual BOOL ifOpen() = 0;
public:
    virtual WORD getNodeId() = 0;
};