#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/button/CommandXboxController.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc2/command/Commands.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/BooleanTopic.h>

#include "subsystems/Drivetrain.h"
#include "subsystems/Arm.h"
#include "commands/DriveBackThenBalance.h"

class RobotContainer {
public:
	RobotContainer();
	frc2::Command* GetAutonomousCommand();
	void UpdateControllerLogging();

private:
	frc2::CommandXboxController controller{0};
	frc2::CommandXboxController controller2{1};
	
	Drivetrain drivetrain;
	Arm arm;

	void ConfigureButtonBindings();

	nt::NetworkTableInstance networkTableInst;
	std::shared_ptr<nt::NetworkTable> driveTable;
	nt::BooleanEntry startAutoBalance;

	void InitControllerLogging();

	//Autonomous
	frc::SendableChooser<frc2::Command*> autonomousChooser;
	DriveBackThenBalance driveBackThenBalance{&drivetrain};


	//Controller logging
	nt::GenericEntry* a;
	nt::GenericEntry* b;
	nt::GenericEntry* x;
	nt::GenericEntry* y;
	nt::GenericEntry* lb;
	nt::GenericEntry* rb;
	nt::GenericEntry* lt;
	nt::GenericEntry* rt;
	nt::GenericEntry* pov;
	nt::GenericEntry* back;
	nt::GenericEntry* start;
	nt::GenericEntry* lx;
	nt::GenericEntry* ly;
	nt::GenericEntry* rx;
	nt::GenericEntry* ry;
	nt::GenericEntry* ls;
	nt::GenericEntry* rs;
};