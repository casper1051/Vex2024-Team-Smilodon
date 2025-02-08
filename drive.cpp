#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.



// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}



void vexcodeInit() {

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}

#pragma endregion VEXcode Generated Robot Configuration
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Team Smiliodon                                            */
/*    Created:      02/04/2025                                                */
/*    Description:  Basic Driving w/ quadr. power curve and basic skill auto  */
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

// Clamp motor or servo (assuming it's a motor for simplicity)
vex::digital_out clamp = digital_out(Brain.ThreeWirePort.A);

bool ramp_enabled = false;
bool is_ramp_spinning = false;

// Main controller
vex::controller Controller = controller(primary);

void onevent_Controller1ButtonL1_pressed_0() {
  clamp.set(false);
}

void onevent_Controller1ButtonL2_pressed_0() {
  clamp.set(true);
}

void clamp_down() {
  clamp.set(false);
}

void clamp_up() {
  clamp.set(true);
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
        // Button B: Auto drive sequence
        if (Controller.ButtonB.pressing()) {
            motor_group leftMotors = motor_group(left_motor, left_motor2);
            motor_group rightMotors = motor_group(right_motor, right_motor2);
            motor_group rampMotors = motor_group(ramp_motor, second_ramp_motor);
            drivetrain Drivetrain = drivetrain(leftMotors, rightMotors, 259.34, 320, 40, mm, 1.8);
            Drivetrain.setDriveVelocity(100, percent);
            Drivetrain.setTurnVelocity(50, percent);

            Drivetrain.driveFor(2.5, inches); // Move to the alliance stake
            rampMotors.spin(forward, 100, percent); // Put preload on the stake
            task::sleep(8000);
            Drivetrain.setDriveVelocity(60, percent);
            Drivetrain.driveFor(-15, inches); // Back away from the stake
            clamp_up(); // Get the clamp ready
            Drivetrain.turnFor(-124, degrees); // Turn to face mogo
            Drivetrain.driveFor(30, inches); // Get to the mogo
            clamp_down(); // Clamp the mogo
            Drivetrain.turnFor(-249, degrees); // Turn 180
            Drivetrain.driveFor(-14, inches); // Get the first ring
            task::sleep(2000);
            Drivetrain.driveFor(-17, inches); // Get the second ring
            task::sleep(500);
            Drivetrain.driveFor(10, inches); // Back away
            Drivetrain.turnFor(300, degrees); // Turn to face corner
            Drivetrain.driveFor(17, inches); // Put mogo in corner
            task::sleep(2000);
            clamp_up(); // Unclamp mogo
            task::sleep(200);
            Drivetrain.driveFor(-11, inches); // Exit corner

            // Next half of auto
            Drivetrain.turnFor(181, degrees); // Turn to face next mogo
            Drivetrain.driveFor(73, inches); // Get to next mogo
            clamp_down(); // Clamp the mogo
            Drivetrain.turnFor(-248, degrees); // Turn 180
            Drivetrain.driveFor(-22, inches); // Get the first ring
            task::sleep(2000);
            Drivetrain.driveFor(-17, inches); // Get the second ring
            task::sleep(500);
            Drivetrain.driveFor(10, inches); // Back away
            Drivetrain.turnFor(-300, degrees); // Turn to face corner
            Drivetrain.driveFor(21, inches); // Put mogo in corner
            task::sleep(2000);
            clamp_up(); // Unclamp mogo
            task::sleep(200);
            Drivetrain.driveFor(-20, inches); // Exit corner


            // to go forward/backward use: Drivetrain.driveFor(num, inches);
            // to turn use: Drivetrain.turnFor(num, degrees);

            task::sleep(5000);
        }

        // Creates variables for computational handling
        int leftcalc;
        int rightcalc;

        // Because of squaring a number, if statements are used for positive and negative
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
        
        // Sets left and right for easier readability
        int left = leftcalc;
        int right = rightcalc;

        // Check for ramp control button press for continuous spinning
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
