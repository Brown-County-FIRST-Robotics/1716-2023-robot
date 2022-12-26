/*  
	TODO:
	move motor varibles to struct
	remove analog_output
*/
#include <iostream>
#include <string>
#include <math.h>  

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


#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"
#include <cmath>


#define g 9.8065

using namespace frc;

class Robot : public TimedRobot
{
public:
	PigeonIMU pigeon{0};

	WPI_TalonFX back_left{1};
	WPI_TalonFX front_left{0};
	WPI_TalonFX front_right{2};
	WPI_TalonFX back_right{3};

	
	WPI_TalonFX shooter_top{6};
	WPI_TalonFX shooter_bottom{7};
	WPI_TalonSRX shooter_angle_1{10};
	WPI_TalonSRX shooter_angle_2{11};
	

	DoubleSolenoid shooter_solenoid{PneumaticsModuleType::CTREPCM, 0, 1};
	Timer shooter_solenoid_timer;


	MotorControllerGroup shooter_angle{shooter_angle_2, shooter_angle_1};

	MotorControllerGroup shooter{shooter_bottom, shooter_top};
	
	XboxController controller{0};

	AnalogInput potentiometer{0};

	MecanumDrive mecDrive{front_left, back_left, front_right, back_right};

	nt::NetworkTableEntry distance;
	nt::NetworkTableEntry left_right;
	nt::NetworkTableEntry up_down;
	nt::NetworkTableEntry token;

	const double velocity = pow(-5,-5); // TODO: change
	double last_token = -1;

	double angle_threshold = 5.0*3.1415/180; // 5 degrees

	double pigeon_target = -100;// radians

	double sweep_speed = 0.1; // speed when the robot is looking for a fiducial

	bool shooting = false;
	const double pot_m = pow(5,5);
	const double pot_b = pow(5,-5);
	double target_height;
	double target_distance;
	double final_angle;
	bool firing = false;

	void SimulationPeriodic()
	{
	}

	void TeleopPeriodic()
	{	
		if(firing){
			if(shooter_solenoid_timer.HasElapsed(.1_s) && .2_s>shooter_solenoid_timer.Get()){
				shooter_solenoid.Set(DoubleSolenoid::kOff);
			}

			if(shooter_solenoid_timer.HasElapsed(.5_s) && .55_s>shooter_solenoid_timer.Get()){
				shooter_solenoid.Set(DoubleSolenoid::kReverse);
			}

			if(shooter_solenoid_timer.HasElapsed(.6_s) && .7_s>shooter_solenoid_timer.Get()){
				shooter_solenoid_timer.Stop();
				shooter_solenoid_timer.Reset();
				shooter_solenoid.Set(DoubleSolenoid::kOff);
				firing=false;
				shooting=false;
				pigeon_target = -100;
				last_token = -1;
			}
		}

		if(shooting){
			if(((potentiometer.GetVoltage()*pot_m)+pot_b)-final_angle<angle_threshold){
				firing=true;
				shooter_solenoid_timer.Start();
				shooter_solenoid.Set(DoubleSolenoid::kForward);
			}else{
				shooter_angle.Set((((potentiometer.GetVoltage()*pot_m)+pot_b)-final_angle)/30);
			}
			return;
		}

		// check if shooting is possible
		if(last_token!=token.GetDouble(0.0)){ // check for new value
			if(atan2(left_right.GetDouble(0.0),distance.GetDouble(0.0)) < angle_threshold){ //check if angle is accurate enough
				mecDrive.DriveCartesian(0,0,0);
				shooting=true;
				target_distance=distance.GetDouble(0.0); // x
				target_height=up_down.GetDouble(0.0); // y
				final_angle=atan((pow(velocity,2)+pow(pow(velocity,4)-(g*(g*pow(target_distance,2)+2*target_height*pow(velocity,2))),0.5))/(g*target_distance));
				return;
			} 
		}

		if(last_token==token.GetDouble(0.0) && pigeon_target!=-100){ // no new value and pigeon has target
			mecDrive.DriveCartesian(0,0,(pigeon_target-(pigeon.GetYaw()*3.1415/180))/10);
		}else if(last_token!=token.GetDouble(0.0)){ //new value
			std::cout << "Apriltag found\n";
			double dist = distance.GetDouble(0.0);
			double lr = left_right.GetDouble(0.0);
			double theta = atan2(lr,dist);
			pigeon_target = int(pigeon.GetYaw() + 360.0 + theta) % 360;
			last_token=token.GetDouble(0.0);
			return;
		}else if(pigeon_target==-100 && last_token==-1){ //no target and fiducial has not been seen
			mecDrive.DriveCartesian(0,0,sweep_speed);
		}
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


		auto inst = nt::NetworkTableInstance::GetDefault();
		auto table = inst.GetTable("apriltags");
		distance = table->GetEntry("distance");
		left_right = table->GetEntry("left_right");
		up_down = table->GetEntry("up_down");
		token = table->GetEntry("random");
		/*token.AddListener(
			[](const nt::EntryNotification &event)
			{
				//
			}, NT_NOTIFY_UPDATE | NT_NOTIFY_NEW);*/

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
