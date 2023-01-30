#include <frc/smartdashboard/SmartDashboard.h>

#include "RobotContainer.h"
#include "Constants.h"

#include "commands/TeleopDrive.h"
#include "commands/AutoBalance.h"
#include "commands/RasPiDrive.h"
#include "commands/SwitchDriveType.h"

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	ConfigureButtonBindings();
	
	drivetrain.SetDefaultCommand(TeleopDrive(&drivetrain, 
		[this] { return controller.GetLeftY(); }, 
		[this] { return 0; },//controller.GetLeftX(); }, 
		[this] { return controller.GetLeftX(); },//RightX(); }));
		[this] { return controller.GetBButton(); } ));

	//Autonomous:
	autonomousChooser.SetDefaultOption("Drive Back and Auto-level", &driveBackThenBalance);

	frc::SmartDashboard::PutData("Autonomous Routine", &autonomousChooser);
}

void RobotContainer::ConfigureButtonBindings() {
	controller.A().OnTrue(SwitchDriveType(&drivetrain).WithTimeout(SolenoidConst::SOLENOIDSETLENGTH));

	//Drive modes (controlled with D-Pad, cancelled on D-Pad down):
	frc2::Trigger([this] { return controller.GetPOV() == 270; }).OnTrue(TeleopDrive(&drivetrain, 
		[this] { return -controller.GetLeftY(); }, 
		[this] { return 0; },//controller.GetLeftX(); }, 
		[this] { return controller.GetLeftX(); },//RightX(); }));
		[this] { return controller.GetBButton(); } ).Until([this] { return controller.GetPOV() == 180; }));
		//Backwards driving (left on D-Pad)

	frc2::Trigger([this] { return controller.GetPOV() == 90; }).OnTrue(AutoBalance(&drivetrain).Until([this] { return controller.GetPOV() == 180; }));
		//Auto balancing (right on D-Pad)

	frc2::Trigger([this] { return controller.GetPOV() == 0; }).OnTrue(RasPiDrive(&drivetrain).Until([this] { return controller.GetPOV() == 180; }));
		//RasPi control (up on D-Pad)
}

frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
	return autonomousChooser.GetSelected();
}