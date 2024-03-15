#pragma once 
#include "Motor.h"


class MaxonDCMotor : public Motor{

    BOOL ifOpen() override;

public:
    WORD getNodeId() override;
    

public:
    MaxonDCMotor(BOOL Default);
    MaxonDCMotor(char* portName, char* deviceName, char* protocolStackName, char* interfaceName, int operationMode);
    BOOL setOperationMode(char operationMode);

    void printMotorInfo();
    void* getKeyHandle();
    void openDevice();
    void modNodeId(WORD nodeId);

    BOOL setProfileMode(DWORD profileVelocity, DWORD profileAccerleration, DWORD profileDecerleration);
    BOOL Activate();
    BOOL Enable();
    BOOL Disable();

    void Move2Position(long position, bool absolute, bool immediate);
    void MoveWithVelocity(long velocity);
    void Move2Zero();
    int GetPosition();

    ~MaxonDCMotor();

protected:
    void _openTesting();

private:
    BOOL __setMotorType(const unsigned short MotorType);
};