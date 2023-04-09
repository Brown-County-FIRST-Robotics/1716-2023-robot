#pragma once
// #include <units/time.h>
#include <networktables/NetworkTableValue.h>
#include <frc/TimedRobot.h> //units::scalar
#include <frc/DoubleSolenoid.h>

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

	const double PROPORTIONAL_FACTOR = ((.0067 * .6) * 1.9 * 1.5)*.7;
	const double INTEGRAL_FACTOR = (((ULTIMATEGAIN * 1.2) / OSCPERIOD) * .3);
	const double DERIVATIVE_FACTOR = ((ULTIMATEGAIN * OSCPERIOD) * 0.075) * 0;
};

namespace ArmConst
{
	//IDs
	const int SHOULDER_ID = 50;
	const int ELBOW_ID = 51;
	const int CLAW_ID[2] = {2, 3};	//0 is forward, 1 is reverse
	const int SHOULDER_POTENTIOMETER_ID = 0;

	//max speeds
	const double SHOULDER_MAX_SPEED = 1;
	const double ELBOW_MAX_OUTPUT = 1;

	//manual control
	const double SHOULDER_JOYSTICK_SPEED = 20;
	const double ELBOW_MANUAL_SPEED = 2.5;
	const double SHOULDER_MANUAL_SPEED = 10;

	//PID
	const double SHOULDER_P = 0.04;

	const double ELBOW_P = 0.0002;
	const double ELBOW_I = 0;//5e-7;
	const double ELBOW_D = 0.0;
	const double ELBOW_FEED_FORWARD = 0; //this is direct feed forward of the setpoint, NOT a constant for the arbitrary feed forward!

	const double ELBOW_ARBITRARY_FEED_FORWARD = 1.0;

	const double ELBOW_IZONE = 0;

/**
 * 
	const double ELBOW_P = 0.0002;
	const double ELBOW_I = 5e-7;
	const double ELBOW_D = 1.0;
	const double ELBOW_FEED_FORWARD = 0.0002; //this is direct feed forward of the setpoint, NOT a constant for the arbitrary feed forward!
//maybe 1.0/5000 for the motor kv at max rpm?

 * 
*/


	//Smartmotion:
	const double CLOSED_LOOP_ERROR = 1.0;
	const int MAX_VELOCITY = 2000; //RPM
	const int MIN_VELOCITY = 15; //RPM
	const int MAX_ACCEL = 6000; //RPM^2
	const double ELBOW_ROTAIONS_TO_ANGLE_RATIO = (12.75 * (64.0 / 14.0)) / 360;

	//shoulder misc.
	const int SHOULDER_EXTREME[2] = {80, 120}; //min, max
	const int SHOULDER_PID_TOLERANCE = 2;

	const frc::DoubleSolenoid::Value CLAW_OPEN = frc::DoubleSolenoid::kForward;
	const frc::DoubleSolenoid::Value CLAW_CLOSED = frc::DoubleSolenoid::kReverse;
};


namespace ArmHeightConst{
	// {shoulder, elbow}
	const double FLOOR[2]={124, -40};
	const double PORTAL[2]={107, -31};
	const double MEDIUM[2]={107, -32};
	const double HIGH[2]={114, -27};
	const double DRIVE[2]={90, -47};
	const double THRESHOLD = 20;
};

namespace AutonomousConst
{
	const double TIMEOUT_DISTANCE = 30; //distance to drive before giving up
	const double ROTATION_DISTANCE = 90; //The distance for the robot to rotate before realizing it's behaving properly
};

namespace LEDConst
{
	const int LENGTH = 300; //amount of lights in the string
	const int ID = 9;
	const int UPDATE_SPEED = 2;
	const int NUM_OF_NIGHT_RIDER_TRAILING_LIGHTS = 7;
};
