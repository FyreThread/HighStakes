/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Target7175                                                */
/*    Created:      10/3/2024, 2:56:54 PM                                     */
/*    Description:  High Stakes 9144G Code                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;

// define your global instances of motors and other devices here

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
}
