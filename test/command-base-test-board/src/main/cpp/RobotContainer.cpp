#include "RobotContainer.h"
#include "Constants.h"

#include <frc2/command/button/JoystickButton.h>
#include <frc/XboxController.h> //used for enumerators
#include <units/time.h>
#include <frc2/command/ParallelRaceGroup.h>
#include <frc/smartdashboard/SmartDashboard.h>

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	ConfigureButtonBindings();

	//Autonomous:
	autonomousChooser.SetDefaultOption("Spin SRX Then FX Then Both", &spinSrxThenFxThenBoth);
	autonomousChooser.AddOption("Spin All", &spinAll);
	autonomousChooser.AddOption("Spin TalonSRX", &spinTsrx);
	autonomousChooser.AddOption("Spin TalonFX", &spinTfx);
	frc::SmartDashboard::PutData("Autonomous Routine", &autonomousChooser);
}

void RobotContainer::ConfigureButtonBindings() {
	// Configure your button bindings to commands here
	(JoystickButton(&controller, XboxController::Button::kA)
		&& JoystickButton(&controller, XboxController::Button::kB))
		.WhileActiveOnce(SpinAll{&motors}); //if both are held, spin both
	
	(JoystickButton(&controller, XboxController::Button::kA)
		&& (!JoystickButton(&controller, XboxController::Button::kB)))
		.WhileActiveOnce(SpinTfx{&motors}); //if a is held, but not b, spin Tfx

	(JoystickButton(&controller, XboxController::Button::kB)
		&& (!JoystickButton(&controller, XboxController::Button::kA)))
		.WhileActiveOnce(SpinTsrx{&motors}); //if b is held, but not a, spin Tsrx

	JoystickButton(&controller, XboxController::Button::kY).WhenActive(ToggleSolenoid{&solenoidSubsystem}.WithTimeout(SOLENOIDSETLENGTH));
}

frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
	return autonomousChooser.GetSelected();
}