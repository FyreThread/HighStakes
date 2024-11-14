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

motor wallStakeA = motor(PORT11, ratio18_1, true);
motor wallStakeB = motor(PORT12, ratio18_1, false);
motor_group wallStake = motor_group(wallStakeA, wallStakeB);

motor leftMotorA = motor(PORT1, ratio6_1, true);
motor leftMotorB = motor(PORT2, ratio6_1, true);
motor leftMotorC = motor(PORT3, ratio6_1, true);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB, leftMotorC);
motor rightMotorA = motor(PORT5, ratio6_1, false);
motor rightMotorB = motor(PORT6, ratio6_1, false);
motor rightMotorC = motor(PORT7, ratio6_1, false);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB, rightMotorC);
inertial Inertial = inertial(PORT8);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, Inertial, 219, 304.8, 266.7, mm, 0.75);

digital_out actuator1 = digital_out(Brain.ThreeWirePort.A);
digital_out doinker = digital_out(Brain.ThreeWirePort.H);

controller Controller1 = controller(primary);

competition Competition = competition();

// Driver Control Code
void driverControl() {
  while(1==1) {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis4
      // right = Axis3 - Axis4
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis4.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis4.position() - Controller1.Axis1.position();

        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);

        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
  }
}

// Control Functions
void intakeForward() {
    intake.spin(forward);
}
void intakeReverse() {
    intake.spin(reverse);
}
void intakeStop() {
    intake.stop();
}
void wallStakePrime() {
    wallStake.spinFor(180, degrees);
}
void wallStakeScore() {
    wallStake.spinFor(500, degrees);
}
void wallStakeForward() {
    wallStake.spin(forward);
}
void wallStakeStow() {
    wallStake.spin(reverse);
}
void wallStakeStop() {
    wallStake.stop();
}
void mobileGrab() {
    actuator1.set(true);
}
void mobileRelease() {
    actuator1.set(false);
}
void doinkerOut() {
    doinker.set(true);
}
void doinkerIn() {
    doinker.set(false);
}

// Starting Settings
void initalize() {
    Inertial.calibrate();
    Inertial.setHeading(0, degrees);
    actuator1.set(false);
    doinker.set(false);
    Drivetrain.setDriveVelocity(150, percent);
    Drivetrain.setTurnVelocity(70, percent);
    Drivetrain.setStopping(brake);
    intake.setVelocity(150, percent);
    wallStake.setVelocity(150,percent);
    wallStake.setStopping(brake);
}

// Button Controls
void buttons() {
  Controller1.ButtonR2.pressed(intakeForward);
    Controller1.ButtonR2.released(intakeStop);
    Controller1.ButtonL2.pressed(intakeReverse);
    Controller1.ButtonL2.released(intakeStop);
    Controller1.ButtonA.pressed(wallStakePrime);
    Controller1.ButtonX.pressed(wallStakeScore);
    Controller1.ButtonY.pressed(wallStakeStow);
    Controller1.ButtonY.released(wallStakeStop);
    Controller1.ButtonB.pressed(wallStakeForward);
    Controller1.ButtonB.released(wallStakeStop);
    Controller1.ButtonL1.pressed(mobileGrab);
    Controller1.ButtonR1.pressed(mobileRelease);
    Controller1.ButtonLeft.pressed(doinkerOut);
    Controller1.ButtonUp.pressed(doinkerIn);
}
//By Gllitchh

float ActualHeading, TargetHeading, error, MotorSpeed;

// Left P Loop 
void turnLeftDegrees(double targetDegrees) {
  // Proportional gain (Turn to much, make smaller, Turn to little, make bigger)
  double kP = 0.432;//432

  // Dead zone for the loop
  int deadzone = 2;

  //The slowist speed the Loop start with.
  double minMotorSpeed = 5.0;

  // Calculate target heading
  double targetHeading = Drivetrain.heading() - targetDegrees;

  // Ensure targetHeading is within the range [0, 360)
  if (targetHeading < 0) {
    targetHeading += 360.0;
  } else if (targetHeading >= 360.0) {
    targetHeading -= 360.0;
  }

  // Turn left until the target heading is reached
  while (true) {
    // Calculate error
    double error = targetHeading - Drivetrain.heading();
    
    // Ensure error is within the range [-180, 180)
    if (error < -180.0) {
      error += 360.0;
    } else if (error >= 180.0) {
      error -= 360.0;
    }

    // Apply dead zone
    if (fabs(error) < deadzone) {
      error = 0.0;
    }
    
    // Calculate motor speed based on error
    double motorSpeed = error * kP;

    // Decrease motor speed as error decreases
    motorSpeed *= fabs(error / targetDegrees);

  if (fabs(motorSpeed) < minMotorSpeed) {
    motorSpeed = copysign(minMotorSpeed, motorSpeed);
  }

    // Set the motor speeds for left turn
    LeftDriveSmart.setVelocity(motorSpeed, percent);
    RightDriveSmart.setVelocity(-motorSpeed, percent); // Reverse the direction

    // Spin the motors for left turn
    LeftDriveSmart.spin(forward);
    RightDriveSmart.spin(forward);

    // Break the loop if the target heading is reached
    if (fabs(error) < 2.0) {
      Drivetrain.setDriveVelocity(150.0, percent);
      Drivetrain.stop();
      break;
    }
    
    // Wait for a short duration
    wait(5, msec);
  }

  // Stop the drivetrain
  Drivetrain.stop();
}

//Right P loop
void turnRightDegrees(double targetDegrees) {
  // Proportional gain (Turn to much, make smaller, Turn to little, make bigger)
  double kP = 0.432;//432

  // Dead zone for the Loop
  int deadzone = 2;

  //The slowist speed the loop will start with  
  double minMotorSpeed = 5.0;

  // Calculate target heading
  double targetHeading = Drivetrain.heading() + targetDegrees;

  // Ensure targetHeading is within the range [0, 360)
  if (targetHeading < 0) {
    targetHeading += 360.0;
  } else if (targetHeading >= 360.0) {
    targetHeading -= 360.0;
  }

  // Turn right until the target heading is reached
  while (true) {
    // Calculate error
    double error = targetHeading - Drivetrain.heading();
    
    // Ensure error is within the range [-180, 180)
    if (error < -180.0) {
      error += 360.0;
    } else if (error >= 180.0) {
      error -= 360.0;
    }

    // Apply dead zone
    if (fabs(error) < deadzone) {
      error = 0.0;
    }
    
    // Calculate motor speed based on error
    double motorSpeed = error * kP;

    // Decrease motor speed as error decreases
    motorSpeed *= fabs(error / targetDegrees);

   if (fabs(motorSpeed) < minMotorSpeed) {
     motorSpeed = copysign(minMotorSpeed, motorSpeed);
    }

    // Set the motor speeds for right turn
    LeftDriveSmart.setVelocity(motorSpeed, percent);
    RightDriveSmart.setVelocity(-motorSpeed, percent); // Reverse the direction

    // Spin the motors for right turn
    LeftDriveSmart.spin(forward);
    RightDriveSmart.spin(forward);

    // Break the loop if the target heading is reached
    if (fabs(error) < 2.0) {
      Drivetrain.stop();
      Drivetrain.setDriveVelocity(150.0, percent);
      break;
    }
    
    // Wait for a short duration
    wait(5, msec);
  }

  // Stop the drivetrain
  Drivetrain.stop();
}