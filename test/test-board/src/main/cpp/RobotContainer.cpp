#include "RobotContainer.h"
#include "Constants.h"

#include <frc2/command/button/JoystickButton.h>
#include <frc/XboxController.h> //used for enumerators
#include <units/time.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/CommandPtr.h>
#include <utility>

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	ConfigureButtonBindings();
	

	//Autonomous:
	//autonomousChooser.SetDefaultOption("Spin SRX Then FX Then Both", SpinTsrx(&motors).WithTimeout(2_s).AndThen(SpinTfx(&motors).WithTimeout(2_s).AndThen(SpinAll(&motors).WithTimeout(2_s))).get());
	autonomousChooser.SetDefaultOption("Spin All", &spinAll);
	autonomousChooser.AddOption("Spin TalonSRX", &spinTsrx);
	autonomousChooser.AddOption("Spin TalonFX", &spinTfx);
	frc::SmartDashboard::PutData("Autonomous Routine", &autonomousChooser);
}

void RobotContainer::ConfigureButtonBindings() {
	// Configure your button bindings to commands here
	(controller.A()
		&& controller.B())
		.WhileTrue(SpinAll(&motors).ToPtr()); //if both are held, spin both
	
	(controller.A()
		&& (!controller.B()))
		.WhileTrue(SpinTfx(&motors).ToPtr()); //if a is held, but not b, spin Tfx

	(controller.B()
		&& (!controller.A()))
		.WhileTrue(SpinTsrx(&motors).ToPtr()); //if b is held, but not a, spin Tsrx

	controller.Y().OnTrue(ToggleSolenoid(&solenoidSubsystem).WithTimeout(SOLENOIDSETLENGTH));
}

frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
	return autonomousChooser.GetSelected();
}