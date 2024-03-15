#include "MaxonDCMotor.h"
#include "Toolkits.h"
#include "Definitions.h"
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/times.h>
#include <sys/time.h>


unsigned short MaxonDCMotor::Motor::deviceNum = 0;
char MaxonDCMotor::Motor::deviceName[] = "EPOS4";
char MaxonDCMotor::Motor::protocolStackName[] = "MAXON SERIAL V2";
char MaxonDCMotor::Motor::interfaceName[] = "USB";


void MaxonDCMotor::_openTesting(){
    Toolkits::stdMsgPrintOut("Creating a maxon DC motor object...", *this);
    if (errorCode == 0){
        unsigned int lBaudrate = 0;
        unsigned int lTimeout = 0;

        if (VCS_GetProtocolStackSettings(m_keyHandle, &lBaudrate, &lTimeout, &errorCode) &&
            VCS_SetProtocolStackSettings(m_keyHandle, m_baudrate, lTimeout, &errorCode) &&
            VCS_GetProtocolStackSettings(m_keyHandle, &lBaudrate, &lTimeout, &errorCode)){
            
            Toolkits::stdMsgPrintOut("Created Successfully", *this);
            printMotorInfo();
            cout << endl;

            if (VCS_SetOperationMode(m_keyHandle, m_NodeId, m_operationMode, &errorCode)) {
                Toolkits::stdMsgPrintOut("Mode Setting", *this);
                cout << "Set Operation Mode : " << m_operationMode << endl;
            }
        }
    } else {
        m_keyHandle = 0;
        Toolkits::stdMsgPrintOut("Failed to open, Check your device or other parameters", *this);
        Toolkits::errorCodePrintOut(errorCode, *this);
        deviceNum--;
    }
}

// lib settings
MaxonDCMotor::MaxonDCMotor(BOOL Default){
    m_keyHandle = NULL;
    m_NodeId = ++deviceNum;
    m_portName = (interfaceName + to_string(deviceNum-1)).data();
    m_baudrate = 1000000;
    m_operationMode = OMD_PROFILE_POSITION_MODE;
    m_ifDefault = 1;
    Toolkits::stdMsgPrintOut("Default Parameters Set Successfully!", *this);
    m_keyHandle = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, m_portName, &errorCode);
    _openTesting();
}


MaxonDCMotor::MaxonDCMotor(char* portName, char* deviceName, char* protocolStackName, char* interfaceName, int operationMode) {
    HANDLE keyHandle = 0;
    m_NodeId = ++deviceNum;
    keyHandle = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, portName, &errorCode);
    Toolkits::stdMsgPrintOut("Creating a maxon DC motor object...", *this);
    
    if (errorCode == 0){
        unsigned int lBaudrate = 0;
        unsigned int lTimeout = 0;

        if (VCS_GetProtocolStackSettings(keyHandle, &lBaudrate, &lTimeout, &errorCode) &&
            VCS_SetProtocolStackSettings(keyHandle, m_baudrate, lTimeout, &errorCode) &&
            VCS_GetProtocolStackSettings(keyHandle, &lBaudrate, &lTimeout, &errorCode)){
            m_keyHandle = keyHandle;
            m_portName = portName;
            m_baudrate = lBaudrate;

            Toolkits::stdMsgPrintOut("Created Successfully", *this);
            printMotorInfo();
            cout << endl;

            if (VCS_SetOperationMode(m_keyHandle, m_NodeId, m_operationMode, &errorCode)) {
                Toolkits::stdMsgPrintOut("Mode Setting", *this);
                cout << "Set Operation Mode : " << m_operationMode << endl;
            }
        }
    } else {
        m_keyHandle = 0;
        Toolkits::stdMsgPrintOut("Failed to open, Check your device or other parameters", *this);
        Toolkits::errorCodePrintOut(errorCode, *this);
        deviceNum--;
    }
}



void MaxonDCMotor::openDevice() {
    m_keyHandle = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, m_portName, &errorCode);
    if (!m_keyHandle && __setMotorType(MT_DC_MOTOR)) {
        Toolkits::stdMsgPrintOut("Failed to Open a New DC Motor...", *this);
        Toolkits::errorCodePrintOut(errorCode, *this);
    } else {
        Toolkits::stdMsgPrintOut("Opened Successfully!", *this);
        printMotorInfo();
        cout << endl;
    }
}


BOOL MaxonDCMotor::ifOpen(){
    Toolkits::stdMsgPrintOut("Haven't Opened This Motor...", *this);
    return false;
}


BOOL MaxonDCMotor::setOperationMode(char operationMode) {
    if (m_keyHandle == NULL) return ifOpen();
    if (__setMotorType(MT_DC_MOTOR) && 
        VCS_SetOperationMode(m_keyHandle, m_NodeId, operationMode, &errorCode)) {
        m_operationMode = operationMode;
        Toolkits::stdMsgPrintOut("Operation Mode Set Successfully!", *this);
        return true;
    }
    Toolkits::stdMsgPrintOut("Failed to Set New Operation Mode!", *this);
    Toolkits::errorCodePrintOut(errorCode, *this);
    return false;
}


void MaxonDCMotor::printMotorInfo(){
    cout << "\n===================================================================" << endl;
    cout << "------------------------ Maxon DC Motor Info ----------------------" << endl;
    Toolkits::stdMsgPrintOut("Baudrate = "+to_string(m_baudrate), *this);
    Toolkits::stdMsgPrintOut(m_portName, *this);
    Toolkits::stdMsgPrintOut("Node Id = " + to_string(m_NodeId), *this);
    Toolkits::stdMsgPrintOut("Now Have Opened " + to_string(deviceNum) + " device(s)", *this);
    cout << "[Device Name]         : " << deviceName << endl;
    cout << "[Protocol Stack Name] : " << protocolStackName << endl;
    cout << "[InterFace Name]      : " << interfaceName << endl;
    cout << "--------------------------------------------------------------------" << endl;
    cout << "====================================================================\n" << endl;
    system("read -p 'Press Enter to the main menu...' var");
    system("clear");
}


BOOL MaxonDCMotor::__setMotorType(const unsigned short MotorType){
    if (m_keyHandle == NULL) return ifOpen();
    if (VCS_SetMotorType(m_keyHandle, m_NodeId, MotorType, &errorCode)) {
        Toolkits::stdMsgPrintOut("Maxon DC Motor is Running!", *this);
        return true;
    }
    Toolkits::stdMsgPrintOut("Error Occurs!", *this);
    Toolkits::errorCodePrintOut(errorCode, *this);
    return false;
}


void* MaxonDCMotor::getKeyHandle() { return m_keyHandle; }
WORD MaxonDCMotor::getNodeId() { return m_NodeId; }


void MaxonDCMotor::modNodeId(WORD nodeId) {
    m_NodeId = nodeId;
    Toolkits::stdMsgPrintOut("Warning : Node Id has Changed!", *this);
}


BOOL MaxonDCMotor::setProfileMode(DWORD profileVelocity, DWORD profileAccerleration, DWORD profileDecerleration){
    if (m_keyHandle == NULL) return ifOpen();

    m_profileVelocity = profileVelocity;
    m_profileAccerleration = profileAccerleration;
    m_profileDecerleration = profileDecerleration;

    if (this->m_operationMode == OMD_PROFILE_POSITION_MODE && 
        VCS_SetPositionProfile(m_keyHandle, m_NodeId, profileVelocity, profileAccerleration, profileDecerleration, &errorCode)) {
        Toolkits::stdMsgPrintOut("Position Profile Mode Parameters Setted!", *this);
        return true;
    } else if (this->m_operationMode == OMD_PROFILE_VELOCITY_MODE &&
        VCS_SetVelocityProfile(m_keyHandle, m_NodeId, profileAccerleration, profileDecerleration, &errorCode)){
        Toolkits::stdMsgPrintOut("Velocity Profile Mode Parameters Setted!", *this);
        return true;
    } else {
        Toolkits::stdMsgPrintOut("Error Occurs when Setting Profile Mode!", *this);
        Toolkits::errorCodePrintOut(errorCode, *this);
        return false;
    }

    return false;
}


BOOL MaxonDCMotor::Activate(){
    if (m_keyHandle == NULL) return ifOpen();

    // profile position mode
    if (this->m_operationMode == OMD_PROFILE_POSITION_MODE && 
        VCS_ActivateProfilePositionMode(m_keyHandle, m_NodeId, &errorCode)) {
        Toolkits::stdMsgPrintOut("Position Profile Mode Activated!", *this);
        return true;
    } else if (this->m_operationMode == OMD_PROFILE_VELOCITY_MODE &&
        VCS_ActivateProfileVelocityMode(m_keyHandle, m_NodeId, &errorCode)){
        Toolkits::stdMsgPrintOut("Velocity Profile Mode Activated!", *this);
        return true;
    } else {
        Toolkits::stdMsgPrintOut("Error Occurs when Activating Profile Mode!", *this);
        Toolkits::errorCodePrintOut(errorCode, *this);
        return false;
    }
    return false;
}


BOOL MaxonDCMotor::Enable(){
    if (this->Activate() && VCS_SetEnableState(m_keyHandle, m_NodeId, &errorCode)) {
        Toolkits::stdMsgPrintOut("Enabled!", *this);
        return true;
    } else {
        Toolkits::stdMsgPrintOut("Error Occurs Enabling motor!", *this);
        Toolkits::errorCodePrintOut(errorCode, *this);
        return false;
    }
    return false;
}


BOOL MaxonDCMotor::Disable(){
    if (m_keyHandle == NULL) return ifOpen();
    if (VCS_SetDisableState(m_keyHandle, m_NodeId, &errorCode)) {
        Toolkits::stdMsgPrintOut("Disabled!", *this);
        return true;
    } else {
        Toolkits::stdMsgPrintOut("Error Occurs Disabling motor!", *this);
        Toolkits::errorCodePrintOut(errorCode, *this);
        return false;
    }
    return false;
}


void MaxonDCMotor::Move2Position(long position, bool absolute, bool immediate){ 
    if (m_keyHandle == NULL) {cout << "False Operation!" ; return;}
    if (this->m_operationMode == OMD_PROFILE_POSITION_MODE &&
        this->Enable() &&
        VCS_MoveToPosition(m_keyHandle, m_NodeId, position, absolute, immediate, &errorCode)){
        Toolkits::stdMsgPrintOut("Move to Position "+to_string(position), *this);
    } else {
        Toolkits::stdMsgPrintOut("Move Operation Failed!", *this);
    }
}


void MaxonDCMotor::MoveWithVelocity(long velocity){
    if (m_keyHandle == NULL) {cout << "False Operation!" ; return;}
    if (this->m_operationMode == OMD_PROFILE_VELOCITY_MODE &&
        this->Enable() &&
        VCS_MoveWithVelocity(m_keyHandle, m_NodeId, velocity, &errorCode)){
        Toolkits::stdMsgPrintOut("Move with" + to_string(velocity), *this);
    } else {
        Toolkits::stdMsgPrintOut("Move Operation Failed!", *this);
    }
}


void MaxonDCMotor::Move2Zero(){
    Toolkits::stdMsgPrintOut("Resetting......", *this);
    cout << "----------------------------------------------------" << endl;
    Disable();
    setOperationMode(OMD_PROFILE_POSITION_MODE);
    Move2Position(0, 1, 1);
    sleep(3);
    Disable();
    cout << "----------------------------------------------------" << endl;
}

int MaxonDCMotor::GetPosition(){
    int position = 0;
    if (VCS_GetPositionIs(m_keyHandle, m_NodeId, &position, &errorCode)) return position;
    if (m_keyHandle == NULL) {cout << "False Operation!" ;}
    return 2147483647;
}


MaxonDCMotor::~MaxonDCMotor() { 
    if (m_keyHandle){
        VCS_CloseDevice(m_keyHandle, &errorCode);
        Toolkits::stdMsgPrintOut("Closing...", *this);
        deviceNum--;
    } else {
        cout << "Motor not exits!" << endl;
    }
}