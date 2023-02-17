#pragma once
// #include <units/time.h>
#include <frc/TimedRobot.h> //units::scalar

namespace SolenoidConst {
	const units::second_t SOLENOIDSETLENGTH = 3_ms;
}

// enum Arm {
// 	SHOOTERMOTOR1ID = 6,
// 	SHOOTERMOTOR2ID = 7,

// 	SHOOTERSOLENOIDFORWARDID = 0,
// 	SHOOTERSOLENOIDREVERSEID = 1,

// 	CLIMBERSOLENOIDFORWARDID = 2,
// 	CLIMBERSOLENOIDREVERSEID = 3
// };

// enum ArmAngle {
// 	ANGLEMOTOR1ID = 10,
// 	ANGLEMOTOR2ID = 11
// };

namespace DrivetrainConst {
	const int FRONTLEFTID = 39;
	const int FRONTRIGHTID = 37;
	const int BACKLEFTID = 32;
	const int BACKRIGHTID = 36;
	
	const int PIGEONID = 20;

	const int HUBID = 42;
	const int SOLENOID0FORWARDID = 0;
	const int SOLENOID0REVERSEID = 1;
	const int SOLENOID1FORWARDID = 2;
	const int SOLENOID1REVERSEID = 3;
	const int SOLENOID2FORWARDID = 4;
	const int SOLENOID2REVERSEID = 5;
	const int SOLENOID3FORWARDID = 6;
	const int SOLENOID3REVERSEID = 7;

	const int WAITTICKS = 3; //solenoids

	const int ACCELERATIONCAP = 3; //the acceleration cap in joystick units/second, higher is faster
};

namespace PIDConst {
	const double ULTIMATEGAIN = .0067;
	const double OSCPERIOD = 3.49;

	const double PROPORTIONALFACTOR = (ULTIMATEGAIN * .6) * 1.9;
	const double INTEGRALFACTOR = (((ULTIMATEGAIN * 1.2) / OSCPERIOD) * .5);
	const double DERIVATIVEFACTOR = ((ULTIMATEGAIN * OSCPERIOD) * 0.075) * 0.8;
};