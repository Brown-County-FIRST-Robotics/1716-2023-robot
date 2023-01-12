#pragma once

#include <frc2/command/Command.h>
#include <frc/XboxController.h>
#include <frc/smartdashboard/SendableChooser.h>

#include "commands/SpinTfx.h"
#include "commands/SpinTsrx.h"
#include "commands/SpinAll.h"
#include "commands/ToggleSolenoid.h"
#include "commands/SpinSrxThenFxThenBoth.h"
#include "subsystems/Motors.h"
#include "subsystems/SolenoidSubsystem.h"

class RobotContainer {
public:
	RobotContainer();
	frc2::Command* GetAutonomousCommand();
private:
	// The robot's subsystems and commands are defined here...
	frc::XboxController controller{0};
	
	Motors motors;
	SolenoidSubsystem solenoidSubsystem;

	//Autonomous
	frc::SendableChooser<frc2::Command*> autonomousChooser; //used to choose routine

	SpinAll spinAll{&motors};
	SpinTfx spinTfx{&motors};
	SpinTsrx spinTsrx{&motors};
	SpinSrxThenFxThenBoth spinSrxThenFxThenBoth{&motors};

	void ConfigureButtonBindings();
};