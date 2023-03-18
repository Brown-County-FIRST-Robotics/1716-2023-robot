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
	InitControllerLogging();

	networkTableInst = nt::NetworkTableInstance::GetDefault();
	driveTable = networkTableInst.GetTable("1716Drive");
	startAutoBalance = driveTable->GetBooleanTopic("startAutoBalance").GetEntry(false);

	ConfigureButtonBindings();
	
	drivetrain.SetDefaultCommand(TeleopDrive(&drivetrain, 
		[this] { return controller.GetLeftY(); }, 
		[this] { return controller.GetLeftX(); }, 
		[this] { return controller.GetRightX(); },
		[this] { return controller.GetLeftTriggerAxis() > 0.2; },
		[this] { return controller.GetAButton(); } ));

	arm.SetDefaultCommand(ArmTeleopControl(&arm, [this] { return controller2.GetRightY(); }, 
		[this] { return controller2.GetLeftY(); }, [this] { return controller2.GetXButton(); }));

	//Autonomous:
	autonomousChooser.SetDefaultOption("Drive Back and Auto-level", &driveBackThenBalance);
	autonomousChooser.AddOption("Back Up", &backUp);
	autonomousChooser.AddOption("Raspberry Pie Control", &rasPiDrive);
	autonomousChooser.AddOption("Nothing", &nothing);

	frc::SmartDashboard::PutData("Autonomous Routine", &autonomousChooser);
}

void RobotContainer::ConfigureButtonBindings() {
	frc2::Trigger([this] { return controller.GetRightTriggerAxis() > .2; }).OnTrue(frc2::InstantCommand([this] {drivetrain.ToggleSolenoid();}, {&drivetrain}).ToPtr());
		//toggle solenoid

	//Drive modes
	controller.B().OnTrue(AutoBalance(&drivetrain)
	 	.Until([this] { return controller.GetBackButtonPressed(); }));
	 	//Auto balancing

	controller.X().OnTrue(RasPiDrive(&drivetrain, &arm)
		.Until([this] { return controller.GetBackButton(); }));
		//RasPi control

	frc2::Trigger([this] { return startAutoBalance.Get(); }) //start auto balance remotely
		.OnTrue(AutoBalance(&drivetrain)
	 	.Until([this] { return !startAutoBalance.Get() || controller.GetBackButtonPressed(); }));
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

bool Nothing::IsFinished() { return true; }

BackUp::BackUp(Drivetrain* subsystem)
{
	AddCommands(
		frc2::ParallelDeadlineGroup(
			frc2::WaitCommand(2_s),
			frc2::StartEndCommand([subsystem] {subsystem->SetSolenoid(frc::DoubleSolenoid::Value::kReverse); subsystem->Drive(-0.3, 0, 0);}, 
				[subsystem] { subsystem->Drive(0, 0, 0); }) //command that backs up
		)
	);
}