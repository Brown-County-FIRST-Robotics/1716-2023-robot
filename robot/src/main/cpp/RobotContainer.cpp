#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/InstantCommand.h>

#include "RobotContainer.h"
#include "Constants.h"

#include "commands/TeleopDrive.h"
#include "commands/AutoBalance.h"
#include "commands/RasPiDrive.h"
#include "commands/AutoSwitchDrive.h"

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	ConfigureButtonBindings();
	
	drivetrain.SetDefaultCommand(TeleopDrive(&drivetrain, 
		[this] { return controller.GetLeftY(); }, 
		[this] { return controller.GetLeftX(); }, 
		[this] { return controller.GetRightX(); },
		[this] { return controller.GetBButton(); } ));

	//Autonomous:
	autonomousChooser.SetDefaultOption("Drive Back and Auto-level", &driveBackThenBalance);

	frc::SmartDashboard::PutData("Autonomous Routine", &autonomousChooser);
}

void RobotContainer::ConfigureButtonBindings() {
	controller.A().OnTrue(frc2::InstantCommand([this] {drivetrain.ToggleSolenoid();}).ToPtr());
		//toggle solenoid

	//Drive modes (controlled with D-Pad, cancelled on D-Pad down) 
	//(uses a VERY hacky solution which sets the default command to preserve the drive mode between solenoid toggles):
	frc2::Trigger([this] { return controller.GetPOV() == 270; }).OnTrue(TeleopDrive(&drivetrain, 
		[this] { return -controller.GetLeftY(); }, 
		[this] { return controller.GetLeftX(); }, 
		[this] { return controller.GetRightX(); },
		[this] { return controller.GetBButton(); } )
		.FinallyDo(
			[this](bool interrupted) { drivetrain.SetDefaultCommand(
				TeleopDrive(&drivetrain, 
				[this] { return -controller.GetLeftY(); }, 
				[this] { return controller.GetLeftX(); }, 
				[this] { return controller.GetRightX(); },
				[this] { return controller.GetBButton(); } )
				.Until([this] { return controller.GetPOV() == 180; })); })
		.Until([this] { return controller.GetPOV() == 180; }));
		//Backwards driving (left on D-Pad)

	 frc2::Trigger([this] { return controller.GetPOV() == 90; }).OnTrue(AutoBalance(&drivetrain)
	 	.FinallyDo(
	 		[this](bool interrupted) { drivetrain.SetDefaultCommand(
	 			AutoBalance(&drivetrain)
	 			.Until([this] { return controller.GetPOV() == 180; })); })
	 	.Until([this] { return controller.GetPOV() == 180; }));
	 	//Auto balancing (right on D-Pad)

	frc2::Trigger([this] { return controller.GetPOV() == 0; }).OnTrue(RasPiDrive(&drivetrain)
		.FinallyDo(
			[this](bool interrupted) { drivetrain.SetDefaultCommand(
				RasPiDrive(&drivetrain)
				.Until([this] { return controller.GetPOV() == 180; })); })
		.Until([this] { return controller.GetPOV() == 180; }));
		//RasPi control (up on D-Pad)

	// controller.Start().OnTrue(AutoSwitchDrive(&drivetrain, 
	// 	[this] { return -controller.GetLeftY(); }, 
	// 	[this] { return controller.GetLeftX(); }, 
	// 	[this] { return controller.GetRightX(); },
	// 	[this] { return controller.GetBButton(); } )
	// 	.FinallyDo(
	// 		[this](bool interrupted) { drivetrain.SetDefaultCommand(
	// 			AutoSwitchDrive(&drivetrain, 
	// 				[this] { return -controller.GetLeftY(); }, 
	// 				[this] { return controller.GetLeftX(); }, 
	// 				[this] { return controller.GetRightX(); },
	// 				[this] { return controller.GetBButton(); } )
	// 			.Until([this] { return controller.GetPOV() == 180; })); })
	// 	.Until([this] { return controller.GetPOV() == 180; }));
}

frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
	return autonomousChooser.GetSelected();
}
