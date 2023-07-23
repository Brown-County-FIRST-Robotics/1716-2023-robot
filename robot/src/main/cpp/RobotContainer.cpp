#include <frc2/command/InstantCommand.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc2/command/StartEndCommand.h>
#include <frc2/command/ParallelDeadlineGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/MecanumControllerCommand.h>
#include "commands/PlaceMobBalance.h"
#include <frc/DriverStation.h>


#include "RobotContainer.h"
#include "Constants.h"

#include "commands/TeleopDrive.h"
#include "commands/AutoBalance.h"
#include "commands/ArmTeleopControl.h"

using frc::XboxController;
using namespace frc2;

RobotContainer::RobotContainer() {
	networkTableInst = nt::NetworkTableInstance::GetDefault();

	floorCone = frc::Shuffleboard::GetTab("Teleop").Add("<floor>", false).WithPosition(10,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	floorCube = frc::Shuffleboard::GetTab("Teleop").Add("[floor]", false).WithPosition(9,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	dsCone = frc::Shuffleboard::GetTab("Teleop").Add("<ds>", false).WithPosition(10,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	dsCube = frc::Shuffleboard::GetTab("Teleop").Add("[ds]", false).WithPosition(9,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();

	placePos[0][0] = frc::Shuffleboard::GetTab("Teleop").Add("(1)", false).WithPosition(0,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[0][1] = frc::Shuffleboard::GetTab("Teleop").Add("(2)", false).WithPosition(1,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[0][2] = frc::Shuffleboard::GetTab("Teleop").Add("(3)", false).WithPosition(2,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[0][3] = frc::Shuffleboard::GetTab("Teleop").Add("(4)", false).WithPosition(3,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[0][4] = frc::Shuffleboard::GetTab("Teleop").Add("(5)", false).WithPosition(4,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[0][5] = frc::Shuffleboard::GetTab("Teleop").Add("(6)", false).WithPosition(5,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[0][6] = frc::Shuffleboard::GetTab("Teleop").Add("(7)", false).WithPosition(6,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[0][7] = frc::Shuffleboard::GetTab("Teleop").Add("(8)", false).WithPosition(7,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[0][8] = frc::Shuffleboard::GetTab("Teleop").Add("(9)", false).WithPosition(8,2).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();

	placePos[1][0] = frc::Shuffleboard::GetTab("Teleop").Add("<1>", false).WithPosition(0,1).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[1][1] = frc::Shuffleboard::GetTab("Teleop").Add("[1]", false).WithPosition(1,1).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[1][2] = frc::Shuffleboard::GetTab("Teleop").Add("<2>", false).WithPosition(2,1).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[1][3] = frc::Shuffleboard::GetTab("Teleop").Add("<3>", false).WithPosition(3,1).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[1][4] = frc::Shuffleboard::GetTab("Teleop").Add("[2]", false).WithPosition(4,1).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[1][5] = frc::Shuffleboard::GetTab("Teleop").Add("<4>", false).WithPosition(5,1).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[1][6] = frc::Shuffleboard::GetTab("Teleop").Add("<5>", false).WithPosition(6,1).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[1][7] = frc::Shuffleboard::GetTab("Teleop").Add("[3]", false).WithPosition(7,1).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[1][8] = frc::Shuffleboard::GetTab("Teleop").Add("<6>", false).WithPosition(8,1).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();

	placePos[2][0] = frc::Shuffleboard::GetTab("Teleop").Add("<7>", false).WithPosition(0,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[2][1] = frc::Shuffleboard::GetTab("Teleop").Add("[4]", false).WithPosition(1,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[2][2] = frc::Shuffleboard::GetTab("Teleop").Add("<8>", false).WithPosition(2,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[2][3] = frc::Shuffleboard::GetTab("Teleop").Add("<9>", false).WithPosition(3,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[2][4] = frc::Shuffleboard::GetTab("Teleop").Add("[5]", false).WithPosition(4,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[2][5] = frc::Shuffleboard::GetTab("Teleop").Add("<10>", false).WithPosition(5,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[2][6] = frc::Shuffleboard::GetTab("Teleop").Add("<11>", false).WithPosition(6,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[2][7] = frc::Shuffleboard::GetTab("Teleop").Add("[6]", false).WithPosition(7,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();
	placePos[2][8] = frc::Shuffleboard::GetTab("Teleop").Add("<12>", false).WithPosition(8,0).WithWidget(frc::BuiltInWidgets::kToggleButton).GetEntry();

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
	autonomousChooser.SetDefaultOption("Nothing", AutoRoutine::NOTHING);
	autonomousChooser.AddOption("Place And Balance", AutoRoutine::PLACE_THEN_BALANCE);
	autonomousChooser.AddOption("Place then leave community", AutoRoutine::PLACE_MOBILITY);
	autonomousChooser.AddOption("Place, leave community, And Balance", AutoRoutine::PLACE_MOB_BALANCE);
	autonomousChooser.AddOption("Place, leave community, And Balance (cable bump side)", AutoRoutine::CABLE_PLACE_MOB_BALANCE);

	frc::Shuffleboard::GetTab("Pre Match").Add("auto routine", autonomousChooser);
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
blue: auto
*/
void RobotContainer::ConfigureButtonBindings() {
	frc2::Trigger([this] { return controller.GetLeftTriggerAxis() > 0.2; }).OnTrue(frc2::InstantCommand([this] {drivetrain.ToggleSolenoid(); led.SetDrivetrainMode(drivetrain.GetSolenoid());}, {&drivetrain, &led}).ToPtr());
		//toggle solenoid

	//Drive modes
	controller.Back().ToggleOnTrue(AutoBalance(&drivetrain).ToPtr());
	
	for(int row=0;row<3;row++){
		for(int col=0;col<10;col++){
			frc2::Trigger([this, row, col] { return placePos[row][col]->GetBoolean(false); }).OnTrue(
				frc2::SequentialCommandGroup(
					frc2::InstantCommand([this, row, col] { placePos[row][col]->SetBoolean(true); }, {}),
					PlacePiece(&drivetrain, &arm, &led, row, col)
				).ToPtr()
			);
		}
	}

}

frc2::Command* RobotContainer::GetAutonomousCommand() { //get the currently selected autonomous command
	frc::Pose2d initpose;
	if(autonomousChooser.GetSelected()==AutoRoutine::CABLE_PLACE_MOB_BALANCE){
		initpose=frc::Pose2d(14.91_m,1.22_m,0_deg);
	}else{
		initpose=DrivetrainConst::INITIAL_POSE;
	}

	if(frc::DriverStation::GetAlliance()==frc::DriverStation::Alliance::kBlue){
		drivetrain.SetPose(frc::Pose2d(325.61_in-initpose.X(), initpose.Y(), 180_deg-initpose.Rotation().Degrees()));
	}
	if(autonomousChooser.GetSelected()==AutoRoutine::NOTHING)
		return &nothing;
	else if(autonomousChooser.GetSelected()==AutoRoutine::PLACE_THEN_BALANCE)
		return &placeAndBalance;
	else if(autonomousChooser.GetSelected()==AutoRoutine::PLACE_MOBILITY)
		return &placeMob;
	else if(autonomousChooser.GetSelected()==AutoRoutine::PLACE_MOB_BALANCE)
		return new PlaceMobBalance(&drivetrain, &arm);
	else if(autonomousChooser.GetSelected()==AutoRoutine::CABLE_PLACE_MOB_BALANCE)
		return new PlaceMobBalance(&drivetrain, &arm, true);
	return nullptr;
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
