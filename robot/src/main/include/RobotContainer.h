#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/button/CommandXboxController.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc2/command/Commands.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/BooleanTopic.h>
#include <frc/PneumaticHub.h>

#include "subsystems/Drivetrain.h"
#include "subsystems/Arm.h"
#include "commands/DriveBackThenBalance.h"
#include "commands/RasPiDrive.h"

class Nothing : public frc2::CommandHelper<frc2::CommandBase, Nothing> { //ignore, used for autonomous
public:
	bool IsFinished() override;
};

class BackUp : public frc2::CommandHelper<frc2::SequentialCommandGroup, BackUp> {
public:
	explicit BackUp(Drivetrain* drive);
};

class RasPiAutonomous : public frc2::CommandHelper<frc2::SequentialCommandGroup, RasPiAutonomous> {
public:
	explicit RasPiAutonomous(Drivetrain* drive);
};

class RobotContainer {
public:
	RobotContainer();
	frc2::Command* GetAutonomousCommand();
	void UpdateControllerLogging();

private:
	frc2::CommandXboxController controller{0};
	frc2::CommandXboxController controller2{1};
	
	frc::PneumaticHub hub{SolenoidConst::HUB_ID};

	Drivetrain drivetrain{hub};
	Arm arm{hub};

	void ConfigureButtonBindings();

	nt::NetworkTableInstance networkTableInst;
	std::shared_ptr<nt::NetworkTable> driveTable;
	nt::BooleanEntry startAutoBalance;

	void InitControllerLogging();

	//Autonomous
	frc::SendableChooser<frc2::Command*> autonomousChooser;
	DriveBackThenBalance driveBackThenBalance{&drivetrain};
	Nothing nothing;
	BackUp backUp{&drivetrain};
	RasPiAutonomous rasPiAutonomous{&drivetrain};

	//Controller logging
	nt::GenericEntry* a;
	nt::GenericEntry* b;
	nt::GenericEntry* x;
	nt::GenericEntry* y;
	nt::GenericEntry* lb;
	nt::GenericEntry* rb;
	nt::GenericEntry* lt;
	nt::GenericEntry* rt;
	nt::GenericEntry* pov;
	nt::GenericEntry* back;
	nt::GenericEntry* start;
	nt::GenericEntry* lx;
	nt::GenericEntry* ly;
	nt::GenericEntry* rx;
	nt::GenericEntry* ry;
	nt::GenericEntry* ls;
	nt::GenericEntry* rs;
};
