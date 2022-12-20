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
	WPI_TalonFX back_left{0};
	nt::NetworkTableEntry robotNumEntry;
	double robotNumber;

	void SimulationPeriodic()
	{
	}

	void TeleopPeriodic()
	{	
		double val = robotNumEntry.GetDouble(0.0);
	//std::cout << "Robot Number: " << val << '\n';
	//std::cout << "Client number: " << robotNumber << '\n';
		back_left.Set(val/3.14);
	
		
	}

	void TeleopInit(){
	auto inst = nt::NetworkTableInstance::GetDefault();
	auto table = inst.GetTable("apriltag");
	robotNumEntry = table->GetEntry("roll");
	robotNumber = (double)(rand() % 100 + 1);
	robotNumEntry.SetDouble(robotNumber);
	robotNumEntry.AddListener(
		[](const nt::EntryNotification &event)
		{
			std::cout << "Hello World\n";
		}, NT_NOTIFY_UPDATE | NT_NOTIFY_NEW);
	}


private:
	
};

#ifndef RUNNING_FRC_TESTS
int main()
{
	return frc::StartRobot<Robot>();
}
#endif