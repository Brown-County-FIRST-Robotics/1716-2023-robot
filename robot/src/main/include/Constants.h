#pragma once
// #include <units/time.h>
#include <frc/TimedRobot.h> //units::scalar

namespace SolenoidConst {
	const units::second_t SOLENOIDSETLENGTH = 3_ms;
}



namespace DrivetrainConst {
	const int FRONTLEFTID = 1;//39;
	const int FRONTRIGHTID = 2;//37;
	const int BACKLEFTID = 3;//32;
	const int BACKRIGHTID = 4;//36;
	
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
	const double PROPORTIONALFACTOR = .006;
	const double INTEGRALFACTOR = 0;
	const double DERIVATIVEFACTOR = 0;
};


namespace ArmConst
{	
	const int SHOULDERID = -1;
	const int ELBOWID = -1;

	const int SHOULDERSWITCHID=-1;
	const int ELBOWSWITCHID=-1;

	const double SHOULDERANGLEGROUND=-1;
	const double SHOULDERANGLERETRACT=-1;
	const double SHOULDERANGLEHIGHNODE=-1;
	const double SHOULDERANGLELOWNODE=-1;
	const double SHOULDERANGLELOWPLATFORM=-1;
	const double SHOULDERANGLEHIGHPLATFORM=-1;

	const double ELBOWANGLEGROUND=-1;
	const double ELBOWANGLERETRACT=-1;
	const double ELBOWANGLEHIGHNODE=-1;
	const double ELBOWANGLELOWNODE=-1;
	const double ELBOWANGLELOWPLATFORM=-1;
	const double ELBOWANGLEHIGHPLATFORM=-1;

	const double ELBOW_P=-1;
	const double SHOULDER_P=-1;

	const double ELBOW_THRESHOLD=-1;
	const double SHOULDER_THRESHOLD=-1;

	const double ELBOW_HOMING_SPEED=-1;
	const double SHOULDER_HOMING_SPEED=-1;

} 
