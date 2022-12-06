/*  
	TODO:
	move motor varibles to struct
	remove analog_output
*/
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
	double back_left_target_pos=0;
	double back_left_pos=0;
	double back_left_previous_pos=0;

	WPI_TalonFX front_left{0};
	double front_left_target_pos=0;
	double front_left_pos=0;
	double front_left_previous_pos=0;
	
	WPI_TalonFX front_right{2};
	double front_right_target_pos=0;
	double front_right_pos=0;
	double front_right_previous_pos=0;
	
	WPI_TalonFX back_right{3};
	double back_right_target_pos=0;
	double back_right_pos=0;
	double back_right_previous_pos=0;
	
	WPI_TalonFX shooter_top{6};
	WPI_TalonFX shooter_bottom{7};
	WPI_TalonSRX shooter_angle_1{10};
	WPI_TalonSRX shooter_angle_2{11};
	
	WPI_TalonFX intake{12};
	double intake_target_pos=0;
	double intake_pos=0;
	double intake_previous_pos=0;

	DoubleSolenoid shooter_solenoid{PneumaticsModuleType::CTREPCM, 0, 1};
	Timer shooter_solenoid_timer;

	DoubleSolenoid intake_solenoid{PneumaticsModuleType::CTREPCM, 2, 3};
	int intake_solenoid_previous_position = shooter_solenoid.kForward;
	Timer intake_solenoid_timer;

	DoubleSolenoid climber_solenoid{PneumaticsModuleType::CTREPCM, 4, 5};
	int climber_solenoid_previous_position = shooter_solenoid.kReverse;
	Timer climber_solenoid_timer;

	MotorControllerGroup shooter_angle{shooter_angle_2, shooter_angle_1};
	double shooter_angle_target_pos=0;
	double shooter_angle_pos=0;
	double shooter_angle_previous_pos=0;
	MotorControllerGroup shooter{shooter_bottom, shooter_top};
	double shooter_target_pos=0;
	double shooter_pos=0;
	double shooter_previous_pos=0;

	AnalogOutput debugging_out{0};
	
	XboxController controller{0};

	GenericHID hid{0};
	
	float driveSpeed = 0.5;
	float shooter_angle_speed = 1;
	float shooter_speed = 1.0;
	bool is_birb_activated = true;
	bool hub_shooting[2] = {false,false}; // [isHubShooting, isMovingToCorrectAngle]
	Timer hub_shooting_timer;
	AnalogInput potentiometer{0};

	Timer timer;
	Timer shooter_timer;

	MecanumDrive mecDrive{front_left, back_left, front_right, back_right};

	bool is_shooting=false;
	double shooter_step=0;
	bool lock_on_mode=false;
	Timer shooting_timer;
	bool firing=false;

	Timer autonomous_timer;
	bool autonomous_shooting=false;
	

	double targetYaw=0;
	double yawFactor=0.05;

	void SimulationPeriodic()
	{
	}

	void TeleopPeriodic()
	{

		double joyX = -controller.GetLeftX();
		double joyR = -controller.GetLeftY();
		double joyY = controller.GetRightX();

		/* deadband gamepad 5% */
		if (fabs(joyR) < 0.05)
			joyR = 0;
		if (fabs(joyX) < 0.05)
			joyX = 0;
		if (fabs(joyY) < 0.05)
			joyY = 0;
		
		targetYaw+=joyR*yawFactor;

		double normalYaw=(pigeon.GetYaw()>0)?pigeon.GetYaw():360.0+pigeon.GetYaw();
		mecDrive.DriveCartesian(joyY*driveSpeed, joyX*driveSpeed, (normalYaw-targetYaw)/180);
	}

	void RobotInit()
	{
		timer.Start();
		front_left.SetNeutralMode(Brake);
		back_left.SetNeutralMode(Brake);
		front_right.SetNeutralMode(Brake);
		back_right.SetNeutralMode(Brake);
		shooter_top.SetInverted(true);
		shooter_angle_1.SetInverted(true);
		shooter_angle_2.SetInverted(true);

	}

	void AutonomousInit(){
		autonomous_timer.Start();
		front_left.Set(-0.3);
		front_right.Set(-0.3);
		back_left.Set(-0.3);
		back_right.Set(-0.3);
		intake_solenoid_timer.Start();
		climber_solenoid_timer.Start();
	}

	void AutonomousPeriodic(){
		if(intake_solenoid_timer.Get()>10_s && intake_solenoid_timer.Get() < 10.1_s){
			intake_solenoid.Set(DoubleSolenoid::kForward);
		}
		if(climber_solenoid_timer.Get()>10_s && climber_solenoid_timer.Get() < 10.1_s){
			climber_solenoid.Set(DoubleSolenoid::kForward);
		}
		if(intake_solenoid_timer.Get()>10.25_s && intake_solenoid_timer.Get() < 10.35_s){
			intake_solenoid.Set(DoubleSolenoid::kOff);
			intake_solenoid_timer.Stop();
			intake_solenoid_timer.Reset();
		}
		if(climber_solenoid_timer.Get()>10.25_s && climber_solenoid_timer.Get() < 10.35_s){
			climber_solenoid.Set(DoubleSolenoid::kOff);
			climber_solenoid_timer.Stop();
			climber_solenoid_timer.Reset();
		}

		if(autonomous_timer.HasElapsed(2_s)){
			front_left.Set(0);
			front_right.Set(0);
			back_left.Set(0);
			back_right.Set(0);
		}/*else{
			front_left.Set(-0.3);
			front_right.Set(-0.3);
			back_left.Set(-0.3);
			back_right.Set(-0.3);
		}*/
	}

private:
	
};

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif