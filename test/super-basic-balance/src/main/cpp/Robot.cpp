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


using namespace frc;

class Robot : public TimedRobot
{
public:
	Pigeon2 pigeon{0};

	WPI_TalonFX back_left{1};
	WPI_TalonFX front_left{0};
	WPI_TalonFX front_right{2};
	WPI_TalonFX back_right{3};

	XboxController controller{0};

	MecanumDrive mecDrive{front_left, back_left, front_right, back_right};
	bool isBalancing=false;
	double p_factor=0.1;
	double i_factor=0.1;
	double d_factor=1;

	double last_pos=0;
	double i_element=0;

	void SimulationPeriodic()
	{
	}

	void TeleopPeriodic()
	{	
		if(controller.GetAButtonPressed()){
			isBalancing=!isBalancing;
		}
		if(!isBalancing){
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

			mecDrive.DriveCartesian(joyY, joyX, joyR);
		}else{
			double pitch=pigeon.GetPitch();
			i_element+=pitch;
			mecDrive.DriveCartesian((pitch*p_factor)+(i_factor*i_element)+(d_factor*(pitch-last_pos)),0,0);
			last_pos=pitch;
		}
		
	}

	void RobotInit()
	{
		front_left.SetNeutralMode(Brake);
		back_left.SetNeutralMode(Brake);
		front_right.SetNeutralMode(Brake);
		back_right.SetNeutralMode(Brake);
	}

	void AutonomousInit(){
	}

	void AutonomousPeriodic(){
		
	}

private:
	
};

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif