/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Team Smiliodon                                            */
/*    Created:      10/20/2024, 2:21:17 PM                                    */
/*    Description:  Basic Driving (Joystick Only)                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain Brain;

// Driving motors
vex::motor left_motor = motor(PORT1, false);
vex::motor right_motor = motor(PORT4, true);
vex::motor ramp_motor = motor(PORT10, false);
bool ramp_enabled=false;
//ima rundomy commit this message here
//create a new motor using:
//vex::motor your_motor_name = motor(PORT(number), <reverse>);
// Main controller
vex::controller Controller = controller(primary);

int main() {
    left_motor.spin(forward);
    right_motor.spin(forward);
    ramp_motor.spin(forward);
    while (true) {
        // Read joystick axis values
        int left = -Controller.Axis3.position(); // Forward/Backward
        int right = -Controller.Axis2.position();    // Turning
        if(Controller.ButtonL2.pressing()){
            ramp_enabled=!ramp_enabled;
            wait(100, msec);
        }
        if(ramp_enabled){
            ramp_motor.setVelocity(100, percent);
        }
        else{
            ramp_motor.setVelocity(0, percent);
        }
        // Use the drive_direct function to control motors
        left_motor.setVelocity(left, percent);
        right_motor.setVelocity(right, percent);

        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}
