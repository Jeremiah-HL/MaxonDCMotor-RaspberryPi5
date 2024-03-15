#include "Motor.h"
#include "MaxonDCMotor.h"
#include "Toolkits.h"
#include "Definitions.h"
#include <bcm2835.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/time.h>
#define DEFAULT 1


char deviceName[] = "EPOS4";
char protocolStackName[] = "MAXON SERIAL V2";
char interfaceName[] = "USB";
char portName1[] = "USB0";
char portName2[] = "USB1";
unsigned int errorCode1;


void demoTestFunc();
void testMotorStatus();


void demoTestFunc(){
    MaxonDCMotor motor_2(DEFAULT);
    MaxonDCMotor motor_1(DEFAULT);
    motor_2.modNodeId(2);
    motor_1.modNodeId(1);
    motor_2.setOperationMode(OMD_PROFILE_VELOCITY_MODE);
    motor_1.setOperationMode(OMD_PROFILE_VELOCITY_MODE);
    
    motor_1.setProfileMode(5000, 10000, 10000);
    motor_2.setProfileMode(5000, 10000, 10000);

    char choice = '0';
    while (cin >> choice){
        if (choice == 'u') {
            motor_1.MoveWithVelocity(5000);
            sleep(1);
        }
        if (choice == 'd') {
            motor_1.MoveWithVelocity(-5000);
            sleep(1);
        }
        motor_1.Disable();
        motor_2.Disable();
    }
    motor_1.Move2Zero();
    motor_2.Move2Zero();
    system("read -p 'Press Enter to Continue...' var");
    system("clear");
}


void testMotorStatus(){
    HANDLE keyHandle1 = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, portName1, &errorCode1);
    HANDLE keyHandle2 = VCS_OpenDevice(deviceName, protocolStackName, interfaceName, portName2, &errorCode1);
    if(VCS_SetMotorType(keyHandle1, 2, MT_DC_MOTOR, &errorCode1)&&
    VCS_SetMotorType(keyHandle2, 1, MT_DC_MOTOR, &errorCode1)) cout << "Two Motors Running Successfully!" << endl;
    cout << VCS_SetOperationMode(keyHandle1, 2, OMD_PROFILE_POSITION_MODE, &errorCode1) << endl;
    cout << VCS_ActivateProfilePositionMode(keyHandle1, 2, &errorCode1) << endl;
    cout << VCS_SetEnableState(keyHandle1, 2, &errorCode1) << endl;
    cout << VCS_MoveToPosition(keyHandle1, 2, 0, 1, 1, &errorCode1) << endl;
    sleep(2);
}


int main(){

    // demoTestFunc();
    // testMotorStatus();

    MaxonDCMotor motor_2(DEFAULT);
    MaxonDCMotor motor_1(DEFAULT);
    system("clear");
    cout << "\n===================================================================" << endl;
    cout << "------------------------ Initializing Motors ----------------------" << endl;
    motor_2.modNodeId(2);
    motor_1.modNodeId(1);
    motor_1.setProfileMode(5000, 10000, 10000);
    motor_2.setProfileMode(5000, 10000, 10000);

    motor_1.Move2Position(0, 1, 1);
    motor_2.Move2Position(0, 1, 1);
    sleep(3);
    motor_1.Disable();
    motor_2.Disable();

    motor_1.setOperationMode(OMD_PROFILE_VELOCITY_MODE);
    motor_2.setOperationMode(OMD_PROFILE_VELOCITY_MODE);
    cout << "--------------------------------------------------------------------" << endl;
    cout << "====================================================================\n" << endl;

    system("read -p 'Press Enter to Continue...' var");

    Toolkits::rpiGPIORunning_keys(motor_1, motor_2);
    motor_1.Move2Zero();
    motor_2.Move2Zero();

    system("read -p 'Press Enter to Continue...' var");
    system("clear");
}