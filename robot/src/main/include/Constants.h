#pragma once
// #include <units/time.h>
#include <networktables/NetworkTableValue.h>
#include <frc/TimedRobot.h> //units::scalar
#include <frc/PneumaticHub.h>

namespace SolenoidConst {
	const units::second_t SET_LENGTH = 3_ms;
	static frc::PneumaticHub hub{10}; //for making solenoids
	const int WAIT_TICKS = 3;
};

namespace DrivetrainConst {
	const int FRONT_LEFT_ID = 1;
	const int FRONT_RIGHT_ID = 2;
	const int BACK_LEFT_ID = 3;
	const int BACK_RIGHT_ID = 4;

	const float MAX_SPEED = 1; //max speed, 0 to 1
	
	const int PIGEON_ID = 20;

	//0 is forward, 1 is reverse
	const int SOLENOID_ID[2] = {0, 1};

	const int ACCELERATION_CAP = 3; //the acceleration cap in joystick units/second, higher is faster
};

namespace PIDConst {
	const double PROPORTIONAL_FACTOR = .006;
	const double INTEGRAL_FACTOR = 0;
	const double DERIVATIVE_FACTOR = 0;
};

namespace ArmConst
{ 
	const int SHOULDER_ID = 50;

	//0 is forward, 1 is reverse
	const int ARM_BRAKE_ID[2] = {2, 3};
	const int ARM_DIRECTION_ID[2] = {4, 5};
	const int CLAW_ID[2] = {6, 7};

	const int SHOULDER_SPEED = .5;
};
