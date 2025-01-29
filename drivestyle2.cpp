/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Team Smiliodon                                            */
/*    Created:      1/25/2025                                               */
/*    Description:  Basic Driving w/ quadr. power curve (for faster robot.    */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include<cmath>

using namespace vex;

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
        // Setup variables for calculations
        int leftcalc;
        int rightcalc;

        // Make negative numbers stay negative after squaring
        if (-Controller.Axis3.position() >= 0) {
            leftcalc = pow(-Controller.Axis3.position()/10, 2.0);
        } else {
            leftcalc = -pow(-Controller.Axis3.position()/10, 2.0);
        }

        if (-Controller.Axis2.position() >= 0) {
            rightcalc = pow(-Controller.Axis2.position()/10, 2.0);
        } else {
            rightcalc = -pow(-Controller.Axis2.position()/10, 2.0);
        }

        // Set variables for easier readability
        int left = leftcalc;
        int right = rightcalc;

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
