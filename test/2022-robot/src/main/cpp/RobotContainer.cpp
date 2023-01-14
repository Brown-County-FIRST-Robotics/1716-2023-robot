#include <frc2/command/button/JoystickButton.h>
#include <units/time.h>
// #include <frc/smartdashboard/SmartDashboard.h>

#include "RobotContainer.h"
#include "Constants.h"

//#include "commands/ToggleClimberSolenoid.h"
//#include "commands/Shoot.h"
#include "commands/DriveCartesian.h"

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	ConfigureButtonBindings();

	drivetrain.SetDefaultCommand(DriveCartesian(&drivetrain, 
		[this] { return controller.GetLeftY(); }, 
		[this] { return controller.GetLeftX(); }, 
		[this] { return controller.GetRightX(); }));

	//Autonomous:
	// autonomousChooser.SetDefaultOption("Spin SRX Then FX Then Both", &spinSrxThenFxThenBoth);
	// autonomousChooser.AddOption("Spin All", &spinAll);
	// autonomousChooser.AddOption("Spin TalonSRX", &spinTsrx);
	// autonomousChooser.AddOption("Spin TalonFX", &spinTfx);
	// frc::SmartDashboard::PutData("Autonomous Routine", &autonomousChooser);
}

void RobotContainer::ConfigureButtonBindings() {
	//JoystickButton(&controller, XboxController::Button::kB).WhenActive(ToggleClimberSolenoid{&arm}.WithTimeout(SolenoidConstants::SOLENOIDSETLENGTH));
}

// frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
// 	return autonomousChooser.GetSelected();
// }