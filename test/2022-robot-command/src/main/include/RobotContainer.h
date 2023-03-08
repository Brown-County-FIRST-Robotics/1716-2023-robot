#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/button/CommandXboxController.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc2/command/Commands.h>

#include "subsystems/Drivetrain.h"
#include "commands/DriveBackThenBalance.h"

class RobotContainer {
public:
	RobotContainer();
	frc2::Command* GetAutonomousCommand();

private:
	frc2::CommandXboxController controller{0};
	
	Drivetrain drivetrain;

	void ConfigureButtonBindings();

	//Autonomous
	frc::SendableChooser<frc2::Command*> autonomousChooser;
	DriveBackThenBalance driveBackThenBalance{&drivetrain};
};