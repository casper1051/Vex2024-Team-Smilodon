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
int Infinity = 1000;
// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain Brain;

// Driving motors
vex::motor left_motor = motor(PORT1, false);
vex::motor right_motor = motor(PORT10, true);
vex::motor left_motor2 = motor(PORT3, false);
vex::motor right_motor2 = motor(PORT2, true);
vex::motor ramp_motor = motor(PORT8, false);
vex::motor second_ramp_motor = motor(PORT9, true);
vex::motor bar_motor = motor(PORT19, false);

// Clamp motor or servo (assuming it's a motor for simplicity)
vex::digital_out clamp = digital_out(Brain.ThreeWirePort.A);

bool ramp_enabled = false;
bool is_ramp_spinning = false;

// Main controller
vex::controller Controller = controller(primary);

void onevent_Controller1ButtonL1_pressed_0() {
  clamp.set(true);
}

void onevent_Controller1ButtonL2_pressed_0() {
  clamp.set(false);
}
void drive_direct(int left_motor_speed, int right_motor_speed, int drive_in_milliseconds) {
    // Set the velocity for both motors
    left_motor.setVelocity(-left_motor_speed, percent);
    right_motor.setVelocity(-right_motor_speed, percent);

    // Spin the motors for the specified duration in milliseconds
    left_motor.spin(forward);
    right_motor.spin(forward);
    vex::task::sleep(drive_in_milliseconds); // Sleep for the given time

    // Stop the motors after the specified duration
    left_motor.stop();
    right_motor.stop();
}
int main() {
    // Set the default speed to 0 to prevent the motors from spinning indefinitely
    left_motor.setVelocity(0, percent);
    right_motor.setVelocity(0, percent);
    left_motor2.setVelocity(0, percent);
    right_motor2.setVelocity(0, percent);
    ramp_motor.setVelocity(0, percent);
    second_ramp_motor.setVelocity(0, percent);

    // Register event handlers for clamp control
    Controller.ButtonL1.pressed(onevent_Controller1ButtonL1_pressed_0);
    Controller.ButtonL2.pressed(onevent_Controller1ButtonL2_pressed_0);

    while (true) {
        if(Controller.ButtonB.pressing()){
            vexDelay(1000);
            drive_direct(100, 100, 3500);
            drive_direct(100, -100, 10);
            vexDelay(1000);
        }
        if(Controller.ButtonUp.pressing()){
            while(bar_motor.position(degrees) > -160){
                bar_motor.setVelocity(-10, percent);
            }
        }
        else{
            while(bar_motor.position(degrees) < 0){
                bar_motor.setVelocity(10, percent);
            }
        }
        if(Controller.ButtonLeft.pressing()){
            bar_motor.resetPosition();
        }
        Brain.Screen.clearScreen();
        Brain.Screen.print(bar_motor.position(degrees));
        // Read joystick axis values
        int left = -Controller.Axis3.position();
        int right = -Controller.Axis2.position();

        // Check for ramp control button press
        if (Controller.ButtonA.pressing() && !is_ramp_spinning) {
            ramp_enabled = !ramp_enabled;
            is_ramp_spinning = true; // Prevent rapid toggling on hold
        } else if (!Controller.ButtonA.pressing()) {
            is_ramp_spinning = false; // Reset the flag when the button is released
        }

        // Set the velocity of the motors based on joystick input
        left_motor.setVelocity(left, percent);
        right_motor.setVelocity(right, percent);
        right_motor2.setVelocity(right, percent);
        left_motor2.setVelocity(left, percent);

        // Control the ramp motor
        if(Controller.ButtonR1.pressing() || Controller.ButtonR2.pressing()){
            ramp_enabled = false;
        }
        if (ramp_enabled || Controller.ButtonR1.pressing()) {
            ramp_motor.setVelocity(100, percent);
            ramp_motor.spin(forward);
            second_ramp_motor.setVelocity(100, percent);
            second_ramp_motor.spin(forward);
        } else if (Controller.ButtonR2.pressing()) {
            ramp_motor.setVelocity(100, percent);
            ramp_motor.spin(reverse);
            second_ramp_motor.setVelocity(100, percent);
            second_ramp_motor.spin(reverse);
        } else {
            ramp_motor.stop();
            second_ramp_motor.stop();
        }

        // Spin the motors based on the set velocity
        left_motor.spin(forward);
        right_motor.spin(forward);
        left_motor2.spin(forward);
        right_motor2.spin(forward);

        // Allow other tasks to run
        this_thread::sleep_for(20);
    }
}
