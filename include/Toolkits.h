#pragma once
#include <iostream>
#include "Motor.h"
#include "MaxonDCMotor.h"
using namespace std;


namespace Toolkits{

void stdMsgPrintOut(string msg, Motor &motor);

void errorCodePrintOut(unsigned int errorCode, Motor &motor);

int ConversePosition2Angle(long position);

int ConversePosition2Displacement(long position);

void rpiGPIORunning_keys(MaxonDCMotor &motor_1, MaxonDCMotor &motor_2);

void RuntimeInfoPanel(MaxonDCMotor &motor_1, MaxonDCMotor &motor_2);
}