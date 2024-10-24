#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"
using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
brain       Brain;

// Define motors and other devices below
motor intake = motor(PORT9, ratio6_1, false);

motor leftMotorA = motor(PORT1, ratio18_1, true);
motor leftMotorB = motor(PORT2, ratio18_1, true);
motor leftMotorC = motor(PORT3, ratio18_1, true);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB, leftMotorC);
motor rightMotorA = motor(PORT5, ratio18_1, false);
motor rightMotorB = motor(PORT6, ratio18_1, false);
motor rightMotorC = motor(PORT7, ratio18_1, false);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB, rightMotorC);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 219, 11.5, 17.5, mm, 1.333);

controller Controller1 = controller(primary);

void driverControl() {
  while(1==1) {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis4
      // right = Axis3 - Axis4
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis4.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis4.position();

        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);

        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
  }
}