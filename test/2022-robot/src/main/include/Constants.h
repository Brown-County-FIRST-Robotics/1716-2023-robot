#pragma once
// #include <units/time.h>
#include <frc/TimedRobot.h> //units::scalar

// namespace Solenoid {
// 	const units::second_t SOLENOIDSETLENGTH = 3_ms;
// }

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

	const int ACCELERATIONCAP = 3; //the acceleration cap in joystick units/second, higher is faster
};

enum PIDConst {
	PROPORTIONALFACTOR = 1,
	INTEGRALFACTOR = 1,
	DERIVATIVEFACTOR = 1
};