#pragma once
// #include <units/time.h>
#include <networktables/NetworkTableValue.h>
#include <frc/TimedRobot.h> //units::scalar

namespace SolenoidConst {
	const units::second_t SET_LENGTH = 3_ms;
	const int WAIT_TICKS = 3;
	const int HUB_ID = 10;
}

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

namespace AutolevelConst {
	const double ULTIMATEGAIN = .0067;
	const double OSCPERIOD = 3.49;

	const double PROPORTIONAL_FACTOR = (.0067 * .6) * 1.9 * 1.5;
	const double INTEGRAL_FACTOR = (((ULTIMATEGAIN * 1.2) / OSCPERIOD) * .5);
	const double DERIVATIVE_FACTOR = ((ULTIMATEGAIN * OSCPERIOD) * 0.075) * 1.5;
};

namespace ArmConst
{
	//IDs
	const int SHOULDER_ID = 50;
	const int ELBOW_ID = 51;
	const int CLAW_ID[2] = {2, 3};	//0 is forward, 1 is reverse
	const int SHOULDER_POTENTIOMETER_ID = 0;

	//max speeds
	const double SHOULDER_MAX_SPEED = 0.5;
	const double ELBOW_MAX_SPEED = 0.2;

	//manual control
	const double SHOULDER_JOYSTICK_SPEED = 0.5;
	const double ELBOW_JOYSTICK_SPEED = .5;

	//PID
	const double SHOULDER_P = 0.02;

	const double ELBOW_P = 0.08;
	const double ELBOW_I = 0.0002;
	const double ELBOW_D = 2.0;

	//shoulder misc.
	const int SHOULDER_EXTREME[2] = {50, 120}; //min, max
	const int SHOULDER_PID_TOLERANCE = 2;
};
