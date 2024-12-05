//START
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
controller Controller1 = controller(primary);
motor Motor8 = motor(PORT8, ratio18_1, false);

motor Motor4 = motor(PORT4, ratio18_1, false);

motor LeftMotor = motor(PORT1, ratio18_1, false);

motor RightMotor = motor(PORT10, ratio18_1, true);




// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       {Rocky Ratia}                                             */
/*    Created:      {August 4, 2024}                                          */
/*    Description:  Basic movement script for the robot, and arm/claw mvoement*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;
void arm_up();
void arm_down();
void claw_open();
void claw_closed();
double sensitivity = 0.1;
int main() {
  //Motor4.spin(forward);
  //Motor8.spin(forward);
  LeftMotor.spin(forward);
  RightMotor.spin(forward);
  Motor4.spin(forward);
  Motor8.spin(forward);
  LeftMotor.setVelocity(0, percent);
  RightMotor.setVelocity(0, percent);
  Motor8.setVelocity(45, percent);
  Motor8.setStopping(brake);
  /*arm_up();
  wait(1, seconds);
  claw_open();
  wait(1, seconds);
  arm_down();
  wait(1, seconds);
  claw_closed();
  wait(1, seconds);
  */
  while(1){
    Brain.Screen.clearScreen();
    LeftMotor.setVelocity(Controller1.Axis3.position()*sensitivity, percent);
    RightMotor.setVelocity(Controller1.Axis2.position()*sensitivity, percent);
    if(Controller1.ButtonL1.pressing()){
      Motor4.setVelocity(25, percent);
    }
    else if(Controller1.ButtonL2.pressing()){
      Motor4.setVelocity(-25, percent);
    }
    else{
      Motor4.setVelocity(0, percent);
    }
    if(Controller1.ButtonR1.pressing()){
      Motor8.setVelocity(25, percent);
    }
    else if(Controller1.ButtonR2.pressing()){
      Motor8.setVelocity(-25, percent);
    }
    else{
      Motor8.setVelocity(0, percent);
    }

    //printf("%f\nLEFT:%ld\nRIGHT:%ld\n",Brain.Battery.voltage(volt), Controller1.Axis3.position(), Controller1.Axis2.position());
  }
}
void claw_open(){
  Motor4.spinToPosition(-250, degrees);
}
void claw_closed(){
  Motor4.spinToPosition(0, degrees);
}
void arm_up(){
  Motor8.spinToPosition(500, degrees);
}
void arm_down(){
  Motor8.spinToPosition(0, degrees);
}
//END
