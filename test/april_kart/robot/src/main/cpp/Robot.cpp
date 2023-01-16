// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.


#include <fmt/core.h>

#include <frc/smartdashboard/SmartDashboard.h>

#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"


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

#include <iostream>


using namespace frc;


class Robot : public TimedRobot
{
public:

	WPI_TalonFX back_left{1};
	WPI_TalonFX front_left{0};
	WPI_TalonFX front_right{2};
	WPI_TalonFX back_right{3};

	MecanumDrive mecDrive{front_left, back_left, front_right, back_right};

	nt::NetworkTableEntry rollEntry;
	nt::NetworkTableEntry distanceEntry;
	double robotNumber;

	void SimulationPeriodic()
	{
	}

	void TeleopPeriodic()
	{	
		mecDrive.DriveCartesian((distanceEntry.GetDouble(0.0)-16.0)/4.0,0,rollEntry.GetDouble(0.0)/3.14);
	
		
	}

	void TeleopInit(){
		auto inst = nt::NetworkTableInstance::GetDefault();
		auto table = inst.GetTable("apriltag");
		rollEntry = table->GetEntry("roll");
		distanceEntry = table->GetEntry("distance");
	}


private:
	
};

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif