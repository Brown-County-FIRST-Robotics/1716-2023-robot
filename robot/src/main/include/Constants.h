#pragma once
// #include <units/time.h>
#include <networktables/NetworkTableValue.h>
#include <frc/TimedRobot.h> //units::scalar
#include <frc/PneumaticHub.h>

namespace SolenoidConst {
	const units::second_t SET_LENGTH = 3_ms;
	static frc::PneumaticHub hub{10}; //for making solenoids
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

	const int WAIT_TICKS = 3; //solenoids

	const int ACCELERATION_CAP = 3; //the acceleration cap in joystick units/second, higher is faster
};

namespace PIDConst {
	const double PROPORTIONAL_FACTOR = .006;
	const double INTEGRAL_FACTOR = 0;
	const double DERIVATIVE_FACTOR = 0;
};

namespace ArmConst
{ 
	//for arrays: 0 is shoulder, 1 is elbow
	const int ID[2] = {-1, -1};

	const int LIMIT_SWITCH_ID[2] = {-1, -1};

	const double GROUND_ANGLE[2] = {-1, -1}; //reaching the ground
	const double SHELF_ANGLE[2] = {-1, -1}; //reaching for a cone on the sliding shelf for pickup
	const double RETRACT_ANGLE[2] = {-1, -1}; //retracting into the robot
	const double HIGH_NODE_ANGLE[2] = {-1, -1}; //cone placement angles
	const double LOW_NODE_ANGLE[2] = {-1, -1};
	const double LOW_PLATFORM_ANGLE[2] = {-1, -1}; //cube placement angles
	const double HIGH_PLATFORM_ANGLE[2] = {-1, -1};

	const double PROPORTIONAL[2] = {1, 1};

	const double THRESHOLD = -1; //error threshold in degrees

	const double ZEROING_SPEED = -1; //speed at which they zero on the limit switches
};
