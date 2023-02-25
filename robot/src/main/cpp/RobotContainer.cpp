#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/InstantCommand.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc2/command/RunCommand.h>

#include "RobotContainer.h"
#include "Constants.h"

#include "commands/TeleopDrive.h"
#include "commands/AutoBalance.h"
#include "commands/RasPiDrive.h"
#include "commands/AutoSwitchDrive.h"

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	InitControllerLogging();

	networkTableInst = nt::NetworkTableInstance::GetDefault();
	driveTable = networkTableInst.GetTable("1716Drive");
	startAutoBalance = driveTable->GetBooleanTopic("startAutoBalance").GetEntry(false);

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
	// frc2::Trigger([this] { return controller.GetPOV() == 270; }).OnTrue(TeleopDrive(&drivetrain, 
	// 	[this] { return -controller.GetLeftY(); }, 
	// 	[this] { return controller.GetLeftX(); }, 
	// 	[this] { return controller.GetRightX(); },
	// 	[this] { return controller.GetBButton(); } )
	// 	.FinallyDo(
	// 		[this](bool interrupted) { drivetrain.SetDefaultCommand(
	// 			TeleopDrive(&drivetrain, 
	// 			[this] { return -controller.GetLeftY(); }, 
	// 			[this] { return controller.GetLeftX(); }, 
	// 			[this] { return controller.GetRightX(); },
	// 			[this] { return controller.GetBButton(); } )
	// 			.Until([this] { return controller.GetPOV() == 180; })); })
	// 	.Until([this] { return controller.GetPOV() == 180; }));
	// 	//Backwards driving

	controller.X().OnTrue(AutoBalance(&drivetrain)
	 	.FinallyDo(
	 		[this](bool interrupted) { drivetrain.SetDefaultCommand(
	 			AutoBalance(&drivetrain)
	 			.Until([this] { return controller.GetPOV() == 180; })); })
	 	.Until([this] { return controller.GetBackButton(); }));
	 	//Auto balancing

	controller.Y().OnTrue(RasPiDrive(&drivetrain)
		.FinallyDo(
			[this](bool interrupted) { drivetrain.SetDefaultCommand(
				RasPiDrive(&drivetrain)
				.Until([this] { return controller.GetPOV() == 180; })); })
		.Until([this] { return controller.GetBackButton(); }));
		//RasPi control

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

	frc2::Trigger([this] { bool b = startAutoBalance.Get(); startAutoBalance.Set(false); return b; }) //start auto balance remotely
		.OnTrue(AutoBalance(&drivetrain)
	 	.FinallyDo(
	 		[this](bool interrupted) { drivetrain.SetDefaultCommand(
				AutoBalance(&drivetrain)
	 				.Until([this] { return controller.GetBackButton(); })); 
			}));
}

frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
	return autonomousChooser.GetSelected();
}

void RobotContainer::InitControllerLogging() {
	a = frc::Shuffleboard::GetTab("Controller").Add("A", controller.GetAButton()).GetEntry();
	b = frc::Shuffleboard::GetTab("Controller").Add("B", controller.GetBButton()).GetEntry();
	x = frc::Shuffleboard::GetTab("Controller").Add("X", controller.GetXButton()).GetEntry();
	y = frc::Shuffleboard::GetTab("Controller").Add("Y", controller.GetYButton()).GetEntry();
	lb = frc::Shuffleboard::GetTab("Controller").Add("Left Bumper", controller.GetLeftBumper()).GetEntry();
	rb = frc::Shuffleboard::GetTab("Controller").Add("Right Bumper", controller.GetRightBumper()).GetEntry();
	lt = frc::Shuffleboard::GetTab("Controller").Add("Left Trigger", controller.GetLeftTriggerAxis()).GetEntry();
	rt = frc::Shuffleboard::GetTab("Controller").Add("Right Trigger", controller.GetRightTriggerAxis()).GetEntry();
	pov = frc::Shuffleboard::GetTab("Controller").Add("POV", controller.GetPOV()).GetEntry();
	back = frc::Shuffleboard::GetTab("Controller").Add("Back", controller.GetBackButton()).GetEntry();
	start = frc::Shuffleboard::GetTab("Controller").Add("Start", controller.GetStartButton()).GetEntry();
	lx = frc::Shuffleboard::GetTab("Controller").Add("Left X", controller.GetLeftX()).GetEntry();
	ly = frc::Shuffleboard::GetTab("Controller").Add("Left Y", -controller.GetLeftY()).GetEntry();
	rx = frc::Shuffleboard::GetTab("Controller").Add("Right X", controller.GetRightX()).GetEntry();
	ry = frc::Shuffleboard::GetTab("Controller").Add("Right Y", -controller.GetRightY()).GetEntry();
	ls = frc::Shuffleboard::GetTab("Controller").Add("Left Stick Button", controller.GetLeftStickButton()).GetEntry();
	rs = frc::Shuffleboard::GetTab("Controller").Add("Right Stick Button", controller.GetRightStickButton()).GetEntry();
}

void RobotContainer::UpdateControllerLogging() {
	a->SetBoolean(controller.GetAButton());
	b->SetBoolean(controller.GetBButton());
	x->SetBoolean(controller.GetXButton());
	y->SetBoolean(controller.GetYButton());
	lb->SetBoolean(controller.GetLeftBumper());
	rb->SetBoolean(controller.GetRightBumper());
	lt->SetDouble(controller.GetLeftTriggerAxis());
	rt->SetDouble(controller.GetRightTriggerAxis());
	pov->SetInteger(controller.GetPOV());
	back->SetBoolean(controller.GetBackButton());
	start->SetBoolean(controller.GetStartButton());
	lx->SetDouble(controller.GetLeftX());
	ly->SetDouble(-controller.GetLeftY());
	rx->SetDouble(controller.GetRightX());
	ry->SetDouble(-controller.GetRightY());
	ls->SetBoolean(controller.GetLeftStickButton());
	rs->SetBoolean(controller.GetRightStickButton());
}
