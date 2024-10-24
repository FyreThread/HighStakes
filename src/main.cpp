/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Alex Caroon                                               */
/*    Created:      10/3/2024, 2:56:54 PM                                     */
/*    Description:  High Stakes 9144G Code                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
using namespace vex;

// Motor controls for buttons and autonomous

void intakeForward() {
    intake.spin(forward);
}
void intakeReverse() {
    intake.spin(reverse);
}
void intakeStop() {
    intake.stop();
}



int main() {
    intake.setVelocity(150, percent);

    Controller1.ButtonR2.pressed(intakeForward);
    Controller1.ButtonR2.released(intakeStop);
    Controller1.ButtonL2.pressed(intakeReverse);
    Controller1.ButtonL2.released(intakeStop);
    driverControl();
}