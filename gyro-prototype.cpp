#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "vex.h"

using namespace vex;

// Brain definition
brain Brain;

// V5 Macros
#define waitUntil(condition) do { wait(5, msec); } while (!(condition))
#define repeat(iterations) for (int iterator = 0; iterator < iterations; iterator++)

// Function to initialize random seed
void initializeRandomSeed() {
    int systemTime = Brain.Timer.systemHighResolution();
    double batteryCurrent = Brain.Battery.current();
    double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);
    srand(int(batteryVoltage + batteryCurrent * 100) + systemTime);
}

// VEXcode initialization
void vexcodeInit() {
    initializeRandomSeed();
}

// Helper function for playing sounds
void playVexcodeSound(const char *soundName) {
    printf("VEXPlaySound:%s\n", soundName);
    wait(5, msec);
}

// Motor and Sensor Definitions
motor left_motor(PORT1, false);
motor right_motor(PORT10, true);
motor left_motor2(PORT3, false);
motor right_motor2(PORT2, true);
motor ramp_motor(PORT8, false);
motor second_ramp_motor(PORT9, true);
inertial Inertia(PORT4);
motor_group leftMotors(left_motor, left_motor2);
motor_group rightMotors(right_motor, right_motor2);
motor_group rampMotors(ramp_motor, second_ramp_motor);
smartdrive Drivetrain(leftMotors, rightMotors, Inertia, 259.34, 320, 40, mm, 1.8);
digital_out clamp(Brain.ThreeWirePort.A);
controller Controller(primary);

// State variables
bool ramp_enabled = false;
bool is_ramp_spinning = false;

// Clamp control functions
void clamp_down() { clamp.set(false); }
void clamp_up() { clamp.set(true); }

// Rotation functions
void rotate_right(int deg) {
    if (deg <= 0) return;
    Inertia.setHeading(2, degrees);
    Drivetrain.turn(left);
    Drivetrain.setTurnVelocity(12, percent);
    while (true){
        if(Inertia.heading(degrees) < deg){

        }
        else{
            break;
        }
    }
    Drivetrain.setTurnVelocity(0, percent);
    task::sleep(1000);
}

void rotate_left(int deg) {
    if (deg <= 0) return;
    Inertia.setHeading(359, degrees);
    Drivetrain.turn(right);
    Drivetrain.setTurnVelocity(12, percent);
    while (true){
        if(Inertia.heading(degrees) > 360 - deg){
            
        }
        else{
            break;
        }
    }
    Drivetrain.setTurnVelocity(0, percent);
    task::sleep(1000);
}

int main() {
    left_motor.setVelocity(0, percent);
    right_motor.setVelocity(0, percent);
    left_motor2.setVelocity(0, percent);
    right_motor2.setVelocity(0, percent);
    ramp_motor.setVelocity(0, percent);
    second_ramp_motor.setVelocity(0, percent);
    Drivetrain.setDriveVelocity(100, percent);
    Drivetrain.setTurnVelocity(25, percent);

    Inertia.calibrate();
    while (Inertia.isCalibrating()) task::sleep(1);
    Controller.rumble(".");

    rotate_left(90);
    Controller.rumble(".");
    while(!Controller.ButtonLeft.pressing()){
        task::sleep(1);
    }
    rotate_right(90);
    Controller.rumble(".");
    while(!Controller.ButtonLeft.pressing()){
        task::sleep(1);
    }
    Controller.ButtonL1.pressed(clamp_down);
    Controller.ButtonL2.pressed(clamp_up);

    while (true) {
        int leftcalc = pow(-Controller.Axis3.position() / 10.0, 2) * (Controller.Axis3.position() > 0 ? 1 : -1);
        int rightcalc = pow(-Controller.Axis2.position() / 10.0, 2) * (Controller.Axis2.position() > 0 ? 1 : -1);

        if (Controller.ButtonDown.pressing()) clamp.set(true);
        if (Controller.ButtonUp.pressing()) clamp.set(false);

        if (Controller.ButtonA.pressing() && !is_ramp_spinning) {
            ramp_enabled = !ramp_enabled;
            is_ramp_spinning = true;
        } else if (!Controller.ButtonA.pressing()) {
            is_ramp_spinning = false;
        }

        left_motor.setVelocity(leftcalc, percent);
        right_motor.setVelocity(rightcalc, percent);
        left_motor2.setVelocity(leftcalc, percent);
        right_motor2.setVelocity(rightcalc, percent);

        if (Controller.ButtonR1.pressing() || Controller.ButtonR2.pressing()) {
            ramp_enabled = false;
        }

        if (ramp_enabled || Controller.ButtonR1.pressing()) {
            ramp_motor.spin(forward, 100, percent);
            second_ramp_motor.spin(forward, 100, percent);
        } else if (Controller.ButtonR2.pressing()) {
            ramp_motor.spin(reverse, 100, percent);
            second_ramp_motor.spin(reverse, 100, percent);
        } else {
            ramp_motor.stop();
            second_ramp_motor.stop();
        }

        left_motor.spin(forward);
        right_motor.spin(forward);
        left_motor2.spin(forward);
        right_motor2.spin(forward);

        this_thread::sleep_for(20);
    }
}
