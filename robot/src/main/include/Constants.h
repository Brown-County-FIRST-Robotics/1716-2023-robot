#pragma once
// #include <units/time.h>
#include <networktables/NetworkTableValue.h>
#include <frc/TimedRobot.h> //units::scalar
#include <frc/DoubleSolenoid.h>
#include <frc/geometry/Pose2d.h>

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
	const auto TANK_MODE = frc::DoubleSolenoid::Value::kForward;
	const auto MECH_MODE = frc::DoubleSolenoid::Value::kReverse;

	//should actually change based on which auto is selected
	const auto INITIAL_POSE = frc::Pose2d(0_m, 0_m, 180_deg);


	//roughly measured on robot
	const auto WHEEL_POS_FL_MECANUM = frc::Translation2d( 25.75_in/2,  18.75_in/2);
	const auto WHEEL_POS_FR_MECANUM = frc::Translation2d( 25.75_in/2, -18.75_in/2);
	const auto WHEEL_POS_BL_MECANUM = frc::Translation2d(-25.75_in/2,  18.75_in/2);
	const auto WHEEL_POS_BR_MECANUM = frc::Translation2d(-25.75_in/2, -18.75_in/2);

	const auto WHEEL_EFFECTIVE_DIAMETER_MECANUM = 0.05411255411255412; //meters per motor encoder unint (rotations for us)


	//roughly measured on robot
	const auto WHEEL_POS_FL_TANK = frc::Translation2d( 15.25_in/2,  17.5_in/2);
	const auto WHEEL_POS_FR_TANK = frc::Translation2d( 15.25_in/2, -17.5_in/2);
	const auto WHEEL_POS_BL_TANK = frc::Translation2d(-15.25_in/2,  17.5_in/2);
	const auto WHEEL_POS_BR_TANK = frc::Translation2d(-15.25_in/2, -17.5_in/2);
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
	const double ELBOW_MANUAL_SPEED = 3;
	const double SHOULDER_MANUAL_SPEED = 30;

	//PID
	const double SHOULDER_P = 0.2;

	const double ELBOW_P = 0.00003;
	const double ELBOW_I = 0;//2e-6;
	const double ELBOW_D = 0.0;
	const double ELBOW_FEED_FORWARD = 0.0002; //this is direct feed forward of the setpoint, NOT a constant for the arbitrary feed forward!

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
	const int MAX_VELOCITY = 5000; //RPM
	const int MIN_VELOCITY = 0; //RPM
	const int MAX_ACCEL = 2000; //RPM^2
	const double ELBOW_ROTAIONS_TO_ANGLE_RATIO = (12.75 * (64.0 / 14.0)) / 360;

	//shoulder misc.
	const int SHOULDER_EXTREME[2] = {80, 120}; //min, max
	const int SHOULDER_PID_TOLERANCE = 2;
	const float ELBOW_IN_LIMIT_POS = -290;

	const frc::DoubleSolenoid::Value CLAW_OPEN = frc::DoubleSolenoid::kForward;
	const frc::DoubleSolenoid::Value CLAW_CLOSED = frc::DoubleSolenoid::kReverse;
};

namespace ArmHeightConst{
	// {shoulder, elbow}
	const double FLOOR[2]={124, -41};
	const double PORTAL[2]={107, -31};
	const double MEDIUM[2]={107, -32};
	const double HIGH[2]={114, -23.5};
	const double DRIVE[2]={90, -47};
	const double THRESHOLD = 5;
	const double HIGHCUBE[2]={114, -27};
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

namespace AutoConst
{
	const double X_PID[3] = {1,1,1}; // TODO: add real values
	const double Y_PID[3] = {1,1,1}; // TODO: add real values
	const double THETA_PID[3] = {1,1,1}; // TODO: add real values
	const double X_TOL = 1; // TODO: add real values(meters)
	const double Y_TOL = 1; // TODO: add real values(meters)
	const double THETA_TOL = 1; // TODO: add real values(degrees)
};
