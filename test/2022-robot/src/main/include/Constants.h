#pragma once
#include <units/time.h>

namespace SolenoidConstants {
	const units::second_t SOLENOIDSETLENGTH = 3_ms;
}

enum ArmConstants {
	SHOOTERMOTOR1ID = 6,
	SHOOTERMOTOR2ID = 7,

	SHOOTERSOLENOIDFORWARDID = 0,
	SHOOTERSOLENOIDREVERSEID = 1,

	CLIMBERSOLENOIDFORWARDID = 2,
	CLIMBERSOLENOIDREVERSEID = 3
};

enum ArmAngleConstants {
	ANGLEMOTOR1ID = 10,
	ANGLEMOTOR2ID = 11
};

enum DrivetrainConstants {
	FRONTRIGHTID = 2,
	FRONTLEFTID = 1,
	BACKRIGHTID = 3,
	BACKLEFTID = 0
};