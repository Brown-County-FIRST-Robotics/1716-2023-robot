#include <frc/smartdashboard/SmartDashboard.h>

#include "RobotContainer.h"

#include "commands/DriveCartesian.h"

RobotContainer::RobotContainer() {
	ConfigureButtonBindings();
	
	drivetrain.SetDefaultCommand(DriveCartesian(&drivetrain, 
		[this] { return -controller.GetLeftY(); }, 
		[this] { return controller.GetLeftX(); }, 
		[this] { return controller.GetRightX(); },
		[this] { return controller.GetBButton(); } ));

	//Autonomous:
	//autonomousChooser.SetDefaultOption("Drive Back and Auto-level", &driveBackThenBalance);
	
	//frc::SmartDashboard::PutData("Autonomous Routine", &autonomousChooser);
}

void RobotContainer::ConfigureButtonBindings() {		

}

//frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
//	return autonomousChooser.GetSelected();
//}