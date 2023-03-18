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
	const int SHOULDER_ID = 50;
	const int ELBOW_ID = 51;

	//0 is forward, 1 is reverse
	const int CLAW_ID[2] = {6, 7};

	const double SHOULDER_SPEED = 1;

	//Analog ID
	const int ARM_POTENTIOMETER_ID = 0;

	const double P = .000001;
	const double I = 0;
	const double D = 0;
	const double MAX_OUTPUT = .15;
};


namespace ArmHeightConst{
	const double FLOOR=0; //TODO: add real values
	const double PORTAL=0; //TODO: add real value
	const double MEDIUM=0; //TODO: add real values
	const double HIGH=0; //TODO: add real values
	const double LOWNODE=0; //TODO: add real values
	const double HIGHNODE=0; //TODO: add real values
	const double THRESHOLD=1; 
};
