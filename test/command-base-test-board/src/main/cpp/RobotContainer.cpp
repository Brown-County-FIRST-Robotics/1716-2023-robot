#include "RobotContainer.h"
#include "Constants.h"

#include <frc/XboxController.h> //used for enumerators
#include <units/time.h>
#include <frc2/command/ParallelRaceGroup.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <iostream>

#include "commands/SpinMotor.h"
#include "commands/ToggleSolenoid.h"

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() : controllerHID{0}, spinMotor{&motor, [this] { return 0.3; }} {
	ConfigureButtonBindings();

	motor.SetDefaultCommand(SpinMotor(&motor, [this] { return controllerHID.GetRightTriggerAxis(); }).ToPtr());

	//Autonomous:
	autonomousChooser.SetDefaultOption("Spin TalonSRX", &spinMotor);
	frc::SmartDashboard::PutData("Autonomous Routine", &autonomousChooser);
}

void RobotContainer::ConfigureButtonBindings() {
	// Configure your button bindings to commands here
	controllerHID.Y().OnTrue(ToggleSolenoid(&solenoidSubsystem).WithTimeout(SOLENOIDSETLENGTH));
}

frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
	return autonomousChooser.GetSelected();
}