#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/button/CommandXboxController.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc2/command/Commands.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/BooleanTopic.h>
#include <frc/PneumaticHub.h>
#include <frc/trajectory/TrajectoryConfig.h>
#include <frc/trajectory/TrajectoryGenerator.h>

#include "subsystems/LED.h"
#include "subsystems/Drivetrain.h"
#include "subsystems/Arm.h"
#include "commands/DriveForwardThenBalance.h"
#include "commands/PlaceAndDriveBack.h"
#include "commands/PlaceThenMobility.h"
#include "commands/LogController.h"
#include "commands/PlacePiece.h"


class Nothing : public frc2::CommandHelper<frc2::CommandBase, Nothing> { //ignore, used for autonomous
public:
	bool IsFinished() override;
};

class DriveForward : public frc2::CommandHelper<frc2::SequentialCommandGroup, DriveForward> {
public:
	explicit DriveForward(Drivetrain* drive);
};
enum AutoRoutine{
	NOTHING=0,
	PLACE_THEN_BALANCE=1,
	PLACE_MOBILITY=2,
	PLACE_MOB_BALANCE=3,
	CABLE_PLACE_MOB_BALANCE=4
};

class RobotContainer {
public:
	RobotContainer();
	frc2::Command* GetAutonomousCommand();
	void Init();
private:
	nt::GenericEntry* placePos[3][9];

	nt::GenericEntry* floorCube;
	nt::GenericEntry* floorCone;
	nt::GenericEntry* dsCone;
	nt::GenericEntry* dsCube;


	frc2::CommandXboxController controller{0};
	frc2::CommandXboxController controller2{1};
	
	frc::PneumaticHub hub{SolenoidConst::HUB_ID};

	Drivetrain drivetrain{hub};
	Arm arm{hub};
	LED led;

	void ConfigureButtonBindings();

	nt::NetworkTableInstance networkTableInst;

	//Autonomous
	frc::SendableChooser<int> autonomousChooser;
	DriveForwardThenBalance driveForwardThenBalance{&drivetrain};
	Nothing nothing;
	DriveForward driveForward{&drivetrain};
	PlaceAndDriveBack placeAndBalance{&drivetrain, &arm};
	PlaceThenMobility placeMob{&drivetrain, &arm};
	LogController controllerLogger{controller}; // Don't log the second controller for now, because we will ditch it later
};
