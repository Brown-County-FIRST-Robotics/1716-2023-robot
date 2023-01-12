#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/button/CommandXboxController.h>
#include <frc/XboxController.h>
#include <frc/smartdashboard/SendableChooser.h>

#include "commands/SpinMotor.h"
#include "commands/ToggleSolenoid.h"
#include "subsystems/Motor.h"
#include "subsystems/SolenoidSubsystem.h"

class RobotContainer {
public:
	RobotContainer();
	frc2::Command* GetAutonomousCommand();
private:
	// The robot's subsystems and commands are defined here...
	frc2::CommandXboxController controllerHID;
	
	Motor motor;
	SolenoidSubsystem solenoidSubsystem;

	//Autonomous
	frc::SendableChooser<frc2::Command*> autonomousChooser; //used to choose routine

	SpinMotor spinMotor;

	void ConfigureButtonBindings();
};