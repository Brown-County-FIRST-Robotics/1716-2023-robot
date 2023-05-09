#include <frc2/command/InstantCommand.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc2/command/StartEndCommand.h>
#include <frc2/command/ParallelDeadlineGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc/smartdashboard/SmartDashboard.h>

#include "RobotContainer.h"
#include "Constants.h"

#include "commands/TeleopDrive.h"
#include "commands/AutoBalance.h"
#include "commands/ArmTeleopControl.h"

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	networkTableInst = nt::NetworkTableInstance::GetDefault();

	floorCone = frc::Shuffleboard::GetTab("Teleop").Add("<floor>", false).WithPosition(11,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	floorCube = frc::Shuffleboard::GetTab("Teleop").Add("[floor]", false).WithPosition(10,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	dsCone = frc::Shuffleboard::GetTab("Teleop").Add("<ds>", false).WithPosition(11,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	dsCube = frc::Shuffleboard::GetTab("Teleop").Add("[ds]", false).WithPosition(10,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();


	ConfigureButtonBindings();
	
	drivetrain.SetDefaultCommand(TeleopDrive(&drivetrain, 
		[this] { return controller.GetLeftY(); }, 
		[this] { return controller.GetLeftX(); }, 
		[this] { return controller.GetRightX(); },
		[this] { return controller.GetRightStickButton(); },
		[this] { return controller.GetLeftStickButton(); },
		[this] { return controller.GetStartButton(); } ));

	arm.SetDefaultCommand(ArmTeleopControl(&arm, 
		[this] { return controller.GetPOV(); }, 
		[this] { return controller.GetRightTriggerAxis() > 0.2; },
		[this] { return controller.GetAButton(); },
		[this] { return controller.GetBButton(); },
		[this] { return controller.GetYButton(); },
		[this] { return controller.GetRightBumper(); },
		[this] { return controller.GetLeftBumper(); }));

	//Autonomous:
	autonomousChooser.SetDefaultOption("Drive Forward and Auto-level", &driveForwardThenBalance);
	autonomousChooser.AddOption("Drive Forward", &driveForward);
	autonomousChooser.AddOption("Nothing", &nothing);
	autonomousChooser.AddOption("Place And Balance", &placeAndBalance);
	autonomousChooser.AddOption("Place then leave community", &placeMob);

	frc::Shuffleboard::GetTab("Pre Match").Add("Autonomous Routine", &autonomousChooser);
	controllerLogger.Schedule();
}

void RobotContainer::Init(){
	drivetrain.SetSolenoid(DrivetrainConst::MECH_MODE);
}
/*
purple: cube
yellow: cone
red: mech
green: tank
*/
void RobotContainer::ConfigureButtonBindings() {
	frc2::Trigger([this] { return controller.GetLeftTriggerAxis() > 0.2; }).OnTrue(frc2::InstantCommand([this] {drivetrain.ToggleSolenoid(); led.SetDrivetrainMode(drivetrain.GetSolenoid());}, {&drivetrain, &led}).ToPtr());
		//toggle solenoid

	//Drive modes
	controller.Back().ToggleOnTrue(AutoBalance(&drivetrain).ToPtr());
	 	//Auto balancing

}

frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
	return autonomousChooser.GetSelected();
}

bool Nothing::IsFinished() { return true; }

DriveForward::DriveForward(Drivetrain* drive)
{
	AddCommands(
		frc2::ParallelDeadlineGroup(
			frc2::WaitCommand(5_s),
			frc2::StartEndCommand([drive] {drive->SetSolenoid(frc::DoubleSolenoid::Value::kReverse); drive->Drive(0.2, 0, 0);}, 
				[drive] { drive->Drive(0, 0, 0); }, {drive}) //command that backs up
		));
}
