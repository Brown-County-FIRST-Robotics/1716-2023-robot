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
	const int FRONTLEFTID = 1;
	const int FRONTRIGHTID = 2;
	const int BACKLEFTID = 3;
	const int BACKRIGHTID = 4;
	
	const int PIGEONID = 20;

	const int HUBID = 42;
	const int SOLENOIDFORWARDID = 0;
	const int SOLENOIDREVERSEID = 1;

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