#pragma once
#include <iostream>

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <networktables/FloatTopic.h>
#include <networktables/BooleanTopic.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

#include "subsystems/Drivetrain.h"
#include "subsystems/Arm.h"

class RasPiDrive : public frc2::CommandHelper<frc2::CommandBase, RasPiDrive> {
public:
	explicit RasPiDrive(Drivetrain *drive, Arm *arm, bool isAutonomous = false);
	
	void Execute() override;

	void End(bool interrupted) override;

	bool IsFinished() override;

private:
	Drivetrain* drivetrain;
	Arm* arm;

	nt::NetworkTableInstance networkTableInst;
	std::shared_ptr<nt::NetworkTable> driveTable;

	nt::FloatSubscriber x;
	nt::FloatSubscriber y;
	nt::FloatSubscriber z;
	nt::BooleanSubscriber isTank;

	bool autonomous; //for running in autonomous mode
	double motorStartPos;
	double rotationStartPos;
	bool hasStopped = false;
	
	nt::BooleanEntry armFloor;
	nt::BooleanEntry armPortal;
	nt::BooleanEntry armMedium;
	nt::BooleanEntry armHigh; 
	nt::BooleanEntry armLowNode;
	nt::BooleanEntry armHighNode;
	nt::BooleanEntry isUnstowing;
	bool isUnstowingPrevState = true;

	nt::BooleanEntry pickupObject;
	nt::BooleanEntry dropObject;
};