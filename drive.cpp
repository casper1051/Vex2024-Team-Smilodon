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
/*    Module:       drive.cpp                                                 */
/*    Author:       Team Smiliodon                                            */
/*    Created:      02/14/2025                                                */
/*    Description:  Basic Driving w/ quadr. power curve, clamp, doinker, and  */
/*                  wallstake capabilities.                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include<cmath>

using namespace vex;

// Driving motors
vex::motor left_motor = motor(PORT1, false);
vex::motor left_motor2 = motor(PORT2, false);
vex::motor right_motor = motor(PORT9, true);
vex::motor right_motor2 = motor(PORT10, true);
vex::motor intake = motor(PORT3, true);
vex::motor hook = motor(PORT8, ratio6_1, false);
vex::motor wallstake1 = motor(PORT5, ratio36_1, true);
vex::motor wallstake2 = motor(PORT6, ratio36_1, false);
vex::optical sensor = optical(PORT4);
vex::motor_group wallstake (wallstake1, wallstake2);
vex::digital_out clamp = digital_out(Brain.ThreeWirePort.A);
vex::digital_out doinker = digital_out(Brain.ThreeWirePort.B);
vex::digital_out lifter = digital_out(Brain.ThreeWirePort.C);

bool ramp_enabled = false;
bool doink = false;
bool toggleteam = false;
bool isred = true;
bool wrong = false;
int buffer = 0;

vex::controller Controller = controller(primary);

void clamp_down() {
  clamp.set(false);
}

void clamp_up() {
  clamp.set(true);
}

void lifter_down() {
  lifter.set(false);
}

void lifter_up() {
  lifter.set(true);
}

void wallstake_ready() {
  wallstake.setVelocity(30, percent);
  wallstake.spinToPosition(32, degrees);
}

void wallstake_score() {
  wallstake.setVelocity(50, percent);
  wallstake.spinToPosition(120, degrees);
}

int main() {
  while (true) {
    left_motor.setVelocity(0, percent);
    left_motor2.setVelocity(0, percent);
    right_motor.setVelocity(0, percent);
    right_motor2.setVelocity(0, percent);
    hook.setVelocity(0, percent);
    intake.setVelocity(0, percent);
    wallstake.setStopping(hold);

    sensor.gestureDisable();
    sensor.setLight(ledState::on);
    sensor.brightness(10);
          
    int left = -Controller.Axis3.position();
    int right = -Controller.Axis2.position();

    left_motor.setVelocity(left, percent);
    left_motor2.setVelocity(left, percent);
    right_motor.setVelocity(right, percent);
    right_motor2.setVelocity(right, percent);

    if(Controller.ButtonR1.pressing() || Controller.ButtonR2.pressing()){
      ramp_enabled = false;
    }

    if (Controller.ButtonRight.pressing() && !doink) {
      doinker = !doinker;
      doink = true;
    } else if (!Controller.ButtonRight.pressing()) {
      doink = false;
    }

    if (Controller.ButtonLeft.pressing() && !toggleteam) {
      isred = !isred;
      toggleteam = true;
    } else if (!Controller.ButtonRight.pressing()) {
      toggleteam = false;
    }

    if (180 < sensor.hue() && sensor.hue() < 220) {
      wrong = true;
    }

    if (wrong) {
      buffer += 1;
      if (buffer > 40) {
        wrong = false;
        buffer = 0;
      }
    }

    if (ramp_enabled || Controller.ButtonR1.pressing()) {
      if (buffer > 10 && wrong) {
        intake.setVelocity(100, percent);
        intake.spin(forward);
        hook.setVelocity(100, percent);
        hook.spin(reverse);
      } else {
      intake.setVelocity(100, percent);
      intake.spin(forward);
      hook.setVelocity(100, percent);
      hook.spin(forward);
      }
    } else if (Controller.ButtonR2.pressing()) {
      intake.setVelocity(100, percent);
      intake.spin(reverse);
      hook.setVelocity(100, percent);
      hook.spin(reverse);
    } else {
      intake.stop();
      hook.stop();
    }

    if (Controller.ButtonL1.pressing()) {
      clamp_down();
    }

    if (Controller.ButtonL2.pressing()) {
      clamp_up();
    }

    if (Controller.ButtonA.pressing()) {
      lifter_down();
    }

    if (Controller.ButtonX.pressing()) {
      lifter_up();
    }

    if (Controller.ButtonB.pressing()) {
      wallstake_ready();
    }

    if (Controller.ButtonY.pressing()) {
      wallstake_score();
    }

    if (Controller.ButtonUp.pressing()) {
      wallstake.setVelocity(40, percent);
      wallstake.spin(forward);
    } else if (Controller.ButtonDown.pressing()) {
      wallstake.setVelocity(-40, percent);
      wallstake.spin(forward);
    } else {
      wallstake.stop();
    }
    left_motor.spin(forward);
    left_motor2.spin(forward);
    right_motor.spin(forward);
    right_motor2.spin(forward);

    this_thread::sleep_for(10);
  }
}
