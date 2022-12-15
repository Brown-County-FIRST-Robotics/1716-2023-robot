#include <iostream>
#include <string>

#include "frc/TimedRobot.h"
#include "frc/Joystick.h"
#include "frc/Solenoid.h"
#include "ctre/Phoenix.h"
#include "frc/motorcontrol/MotorControllerGroup.h"
#include "frc/drive/MecanumDrive.h"
#include "frc/DoubleSolenoid.h"
#include "frc/Timer.h"
#include "frc/PneumaticsControlModule.h"
#include "frc/GenericHID.h"
#include "units/time.h"
#include "frc/XboxController.h"
#include "frc/AnalogInput.h"
#include "frc/AnalogOutput.h"
#include "ctre/phoenix/sensors/WPI_Pigeon2.h"

double normalizeAngle(double ang){
	if(ang<0){
		return ang+360.0;
	}
	return ang;
}

using namespace frc;


class Robot : public TimedRobot
{
public:
	Pigeon2 pigeon{0};

	WPI_TalonFX back_left{1};
	WPI_TalonFX front_left{0};
	WPI_TalonFX front_right{2};
	WPI_TalonFX back_right{3};

	WPI_TalonFX shooter_top{6};
	WPI_TalonFX shooter_bottom{7};
	WPI_TalonSRX shooter_angle_1{10};
	WPI_TalonSRX shooter_angle_2{11};
	
	WPI_TalonFX intake{12};

	DoubleSolenoid shooter_solenoid{PneumaticsModuleType::CTREPCM, 0, 1};

	MotorControllerGroup shooter_angle{shooter_angle_2, shooter_angle_1};

	MotorControllerGroup shooter{shooter_bottom, shooter_top};

	AnalogOutput debugging_out{0};
	
	XboxController controller{0};
	
	float driveSpeed = 0.5;
	AnalogInput potentiometer{0};

	MecanumDrive mecDrive{front_left, back_left, front_right, back_right};

	Timer autonomous_timer;
	bool autonomous_shooting=false;

	void TeleopPeriodic()
	{
		double joyX = -controller.GetLeftX();
		double joyR = -controller.GetLeftY();
		double joyY = controller.GetRightX();

		/* deadband gamepad 5% */
		if (fabs(joyR) < 0.2)
			joyR = 0;
		if (fabs(joyX) < 0.2)
			joyX = 0;
		if (fabs(joyY) < 0.2)
			joyY = 0;

		mecDrive.DriveCartesian(joyY*driveSpeed, joyX*driveSpeed, joyR*driveSpeed);
	}

	void RobotInit()
	{
		front_left.SetNeutralMode(Brake);
		back_left.SetNeutralMode(Brake);
		front_right.SetNeutralMode(Brake);
		back_right.SetNeutralMode(Brake);
		shooter_top.SetInverted(true);
		shooter_angle_1.SetInverted(true);
		shooter_angle_2.SetInverted(true);

	}


private:
	
};

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif