#include <frc2/command/button/JoystickButton.h>
#include <units/time.h>
// #include <frc/smartdashboard/SmartDashboard.h>

#include "RobotContainer.h"

//#include "commands/ToggleClimberSolenoid.h"
//#include "commands/Shoot.h"
#include "commands/DriveCartesian.h"
#include <cstdio>

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	ConfigureButtonBindings();
/*
	drivetrain.SetDefaultCommand(DriveCartesian(&drivetrain, 
		[this] { return controller.GetLeftY(); }, 
		[this] { return controller.GetLeftX(); }, 
		[this] { return controller.GetRightX(); }));
*/
	drivetrain.SetDefaultCommand(DriveCartesian(&drivetrain, 
		[this] { 
			double val = controller.GetLeftY() ;
			double out = val * fabs(val) ;
			printf("strafe fwd %.2f -> %.2f\n", val, out);
			return out;
		 }, 
		[this] { 
			double val = controller.GetLeftX() ;
			double out = val * fabs(val) ;
			printf("strafe side %.2f -> %.2f\n", val, out);
			return out;
		 }, 
		[this] { 			
			double val = controller.GetRightX() ;
			double out = val * fabs(val) ;
			printf("rotate %.2f -> %.2f\n", val, out);
			return out;
		 }));


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