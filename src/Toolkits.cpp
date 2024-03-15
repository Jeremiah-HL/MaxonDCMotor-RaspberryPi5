#include "Toolkits.h"
#include "Motor.h"
#include <wiringPi.h>
#include <unistd.h>
#include <getopt.h>
#define axis_a 5 
#define axis_b 13
#define bend_u 6
#define bend_d 19


void Toolkits::stdMsgPrintOut(string msg, Motor &motor){
    cout << "[Motor Node Id: " << motor.getNodeId() << "]: ";
    cout << msg << endl;
}

void Toolkits::errorCodePrintOut(unsigned int errorCode, Motor &motor){
    string msg = "ErrorCode is as followed : " + to_string(errorCode);
    Toolkits::stdMsgPrintOut(msg, motor);
}

int Toolkits::ConversePosition2Angle(long position){
    return 0;
}

int Toolkits::ConversePosition2Displacement(long position){
    return 0;
}

void Toolkits::RuntimeInfoPanel(MaxonDCMotor &motor_1, MaxonDCMotor &motor_2){
    cout << "===================================================================" << endl;
    cout << "--------------------------- Runtime Info --------------------------" << endl;
    cout << "[Motor Node 1 Info]   :      " << endl; 
    cout << "[Position   Info ]    :      " << motor_1.GetPosition() << endl; 
    cout << "[Angle Conversion]    :      " << Toolkits::ConversePosition2Angle(motor_1.GetPosition()) << endl; 
    cout << "-------------------------------------------------------------------" << endl;
    cout << "[Motor Node 1 Info]   :      " << endl; 
    cout << "[Position   Info    ] :      " << motor_2.GetPosition() << endl; 
    cout << "[Actual Displacement] :      " << Toolkits::ConversePosition2Displacement(motor_2.GetPosition()) << endl; 
    cout << "-------------------------------------------------------------------" << endl;
    cout << "===================================================================\n" << endl;
}

void Toolkits::rpiGPIORunning_keys(MaxonDCMotor &motor_1, MaxonDCMotor &motor_2){
    system("clear");
    cout << "Please Make Sure Two Motors Set Right!" << endl;

    // init gpio
    wiringPiSetupGpio();
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(13, INPUT);
    pinMode(19, INPUT);
    pinMode(26, INPUT);
    pullUpDnControl(5, PUD_DOWN);
    pullUpDnControl(6, PUD_DOWN);
    pullUpDnControl(13, PUD_DOWN);
    pullUpDnControl(19, PUD_DOWN);

    // init flags
    bool axisA = 1;
    bool axisB = 1;
    bool bendU = 1;
    bool bendD = 1;
    
    bool reset_p = 0;

    // other flags
    int count = 0;
    long velocity = 2000;

    while (!reset_p)
    {
        if (digitalRead(5) && axisA) {
            motor_1.MoveWithVelocity(velocity);
            axisA = false;
        }
        if (!digitalRead(5) && !axisA) {motor_1.Disable(); axisA = true;}

        if (digitalRead(13) && bendU) {
            motor_2.MoveWithVelocity(velocity);
            bendU = false;
        }
        if (!digitalRead(13) && !bendU) {motor_2.Disable(); bendU = true;}
        
        if (digitalRead(6) && bendD) {
            motor_2.MoveWithVelocity(-1*velocity);
            bendD = false;
        }
        if (!digitalRead(6) && !bendD) {motor_2.Disable(); bendD = true;}
        
        if (digitalRead(19) && axisB) {
            motor_1.MoveWithVelocity(-1*velocity);
            axisB = false;
        }
        if (!digitalRead(19) && !axisB) {motor_1.Disable(); axisB = true;}

        if (count % 100 == 0) { 
            system("clear");
            RuntimeInfoPanel(motor_1, motor_2);
        }

        if (digitalRead(5) && digitalRead(13)) reset_p = true;

        count++;
        delay(50);
        
    }
}


