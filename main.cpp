/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Team Smiliodon                                            */
/*    Created:      10/20/2024, 2:21:17 PM                                    */
/*    Description:  Basic Driving (Joystick Only)                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <cmath> // Required for std::pow

using namespace vex;

// Global instances and variables
vex::brain Brain;
vex::controller Controller = controller(primary);

vex::motor left_motor(PORT1, false);
vex::motor right_motor(PORT10, true);
vex::motor left_motor2(PORT3, false);
vex::motor right_motor2(PORT2, true);
vex::motor ramp_motor(PORT8, false);
vex::motor second_ramp_motor(PORT9, true);

vex::digital_out clamp(Brain.ThreeWirePort.A);

bool ramp_enabled = false;
bool is_ramp_spinning = false;

// Function to drive motors directly for a specified duration
void drive_direct(int left_motor_speed, int right_motor_speed, int duration_ms) {
    left_motor.spin(forward, -left_motor_speed, percent);
    right_motor.spin(forward, -right_motor_speed, percent);
    task::sleep(duration_ms);
    left_motor.stop();
    right_motor.stop();
}
void clamp_down(){
clamp.set(true);
}
void clamp_up(){
clamp.set(false);
}
// Function to reset all motor velocities
void reset_motor_velocities() {
    left_motor.setVelocity(0, percent);
    right_motor.setVelocity(0, percent);
    left_motor2.setVelocity(0, percent);
    right_motor2.setVelocity(0, percent);
    ramp_motor.setVelocity(0, percent);
    second_ramp_motor.setVelocity(0, percent);
}

int main() {
    reset_motor_velocities();

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



        // Button B: Drive sequence
        if (Controller.ButtonB.pressing()) {
            motor_group leftMotors = motor_group(left_motor, left_motor2);
            motor_group rightMotors = motor_group(right_motor, right_motor2);
            drivetrain Drivetrain = drivetrain(leftMotors, rightMotors, 259.34, 320, 40, mm, 1.8);
            Drivetrain.setDriveVelocity(50, percent);

            Drivetrain.driveFor(2, inches); //Move to the pointy stick

            //Put preload on pointy stick
            ramp_motor.spin(forward, 75, percent);
            second_ramp_motor.spin(forward, 100, percent);
            task::sleep(5000);

            //Slam the ring wall
            Drivetrain.setDriveVelocity(100, percent); //full sped
            Drivetrain.driveFor(-5, inches);
            Drivetrain.driveFor(5, inches);
            Drivetrain.setDriveVelocity(50, percent); //normal sped

            //Get off of wall
            Drivetrain.driveFor(-10, inches);

            //clamp_down();
            //task::sleep(500);

            //Turn to goal and rings
            Drivetrain.turnFor(-130, degrees);
            Drivetrain.driveFor(-36, inches);

            //clamp_up();
            //Drivetrain.turnFor(10, degrees);
            //Drivetrain.driveFor(17, inches);

            //clamp_up();

            task::sleep(5000);
        }

        // Screen feedback
        Brain.Screen.clearScreen();
        Brain.Screen.printAt(50, 50, "%d", left_motor.velocity(rpm));
        
        // Button A: Toggle ramp motor
        if (Controller.ButtonA.pressing() && !is_ramp_spinning) {
            ramp_enabled = !ramp_enabled;
            is_ramp_spinning = true;
        } else if (!Controller.ButtonA.pressing()) {
            is_ramp_spinning = false;
        }

        // Clamp control
        if(Controller.ButtonDown.pressing()){
            clamp.set(true);
        }
        if(Controller.ButtonUp.pressing()){
            clamp.set(false);
        }

        // Motor velocity adjustments
        if (Controller.ButtonL1.pressing() || Controller.ButtonL2.pressing()) {
            left_motor.setVelocity(1, percent);
            right_motor.setVelocity(1, percent);
            left_motor2.setVelocity(1, percent);
            right_motor2.setVelocity(1, percent);
        } else {
            left_motor.setVelocity(left, percent);
            right_motor.setVelocity(right, percent);
            left_motor2.setVelocity(left, percent);
            right_motor2.setVelocity(right, percent);
        }

        // Ramp motor control
        if (Controller.ButtonR1.pressing()) {
            ramp_motor.spin(forward, 100, percent);
            second_ramp_motor.spin(forward, 100, percent);
        } else if (Controller.ButtonR2.pressing()) {
            ramp_motor.spin(reverse, 100, percent);
            second_ramp_motor.spin(reverse, 100, percent);
        } else if (ramp_enabled) {
            ramp_motor.spin(forward, 100, percent);
            second_ramp_motor.spin(forward, 100, percent);
        } else {
            ramp_motor.stop();
            second_ramp_motor.stop();
        }

        // Drive motors
        left_motor.spin(forward);
        right_motor.spin(forward);
        left_motor2.spin(forward);
        right_motor2.spin(forward);

        // Allow other tasks to run
        this_thread::sleep_for(20);
    }
}
