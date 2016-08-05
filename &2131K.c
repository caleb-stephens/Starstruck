#pragma config(Sensor, in1,    liftPot,        sensorPotentiometer)
#pragma config(Motor,  port1,           rightFront,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           rightBack,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           toprightLift,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           bottomrightLift, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           clawRight,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           clawLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           bottomleftLift, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           topleftLift,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           leftFront,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          leftBack,      tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"

#include "parallelDrive.c"
#include "motorGroup.c"

#define clawInBtn Btn6U
#define clawOutBtn Btn6D
#define liftUpBtn Btn5U
#define liftDownBtn Btn5D
#define liftTopBtn Btn7U
#define liftBottomBtn Btn7D

#define liftMax 850
#define liftMin 2250
#define clawStillSpeed 15

parallel_drive drive;
motorGroup lift;
motorGroup claw;

void pre_auton() { bStopTasksBetweenModes = true; }

task autonomous() {
  AutonomousCodePlaceholderForTesting();
}

task usercontrol() {
  initializeDrive(drive, false, 20, 10, 1, 0.92125);
  setLeftMotors(drive, 2, leftFront, leftBack);
  setRightMotors(drive, 2, rightFront, rightBack);

  initializeGroup(lift, 4, toprightLift, bottomrightLift, toprightLift, topleftLift);
  configureButtonInput(lift, liftUpBtn, liftDownBtn, 10, 127, -80);
  attachPotentiometer(lift, liftPot);

  initializeGroup(claw, 2, clawLeft, clawRight);

  bool clawClosed = false;
  int target;
	int prevLiftPos, newLiftPos;

  while (true) {
    driveRuntime(drive);

		//claw
    if (vexRT[clawInBtn] == 1) {
      clawClosed = true;
      setPower(claw, 127);
    } else if (vexRT[clawOutBtn] == 1) {
      clawClosed = false;
      setPower(claw, -127);
    } else {
      setPower(claw, clawClosed ? clawStillSpeed : 0);
    }

    //lift
    if (vexRT[liftUpBtn]==1 || vexRT[liftDownBtn]==1) {
			target = 0;
		} else if (vexRT[liftTopBtn] == 1) {
			target = liftMax;
			prevLiftPos = potentiometerVal(lift);
		} else if (vexRT[liftBottomBtn] == 1) {
			target = liftMin;
			prevLiftPos = potentiometerVal(lift);
		}

		if (target == 0) {
			takeInput(lift);
		} else {
			newLiftPos = potentiometerVal(lift);

			if (sgn(prevLiftPos - target) == sgn(newLiftPos - target)) {
				prevLiftPos = potentiometerVal(lift);
				setPower(lift, prevLiftPos>target ? 127 : -127);
			} else {
				target = 0;
			}
		}
  }
}
