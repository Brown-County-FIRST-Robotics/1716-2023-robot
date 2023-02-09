#include <frc/smartdashboard/SmartDashboard.h>

#include "RobotContainer.h"

#include "commands/DriveCartesian.h"
#include "commands/AutoBalance.h"
#include "commands/RasPiDrive.h"

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	ConfigureButtonBindings();
	
	drivetrain.SetDefaultCommand(DriveCartesian(&drivetrain, 
		[this] { return controller.GetLeftY(); }, 
		[this] { return 0; },//controller.GetLeftX(); }, 
		[this] { return controller.GetLeftX(); },//RightX(); }));
		[this] { return controller.GetBButton(); } ));

	//Autonomous:
	autonomousChooser.SetDefaultOption("Drive Back and Auto-level", &driveBackThenBalance);

	frc::SmartDashboard::PutData("Autonomous Routine", &autonomousChooser);
}

void RobotContainer::ConfigureButtonBindings() {
	controller.X().ToggleOnTrue(DriveCartesian(&drivetrain, 
		[this] { return -controller.GetLeftY(); }, 
		[this] { return 0; },//controller.GetLeftX(); }, 
		[this] { return controller.GetLeftX(); },//RightX(); }));
		[this] { return controller.GetBButton(); } ).WithInterruptBehavior(frc2::Command::InterruptionBehavior::kCancelSelf));

	controller.A().ToggleOnTrue(AutoBalance(&drivetrain).ToPtr());

	controller.Y().ToggleOnTrue(RasPiDrive(&drivetrain).ToPtr());
}

frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
	return autonomousChooser.GetSelected();
}