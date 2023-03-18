#pragma once
#include <iostream>

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <networktables/FloatTopic.h>
#include <networktables/BooleanTopic.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

#include "subsystems/Drivetrain.h"

class RasPiDrive : public frc2::CommandHelper<frc2::CommandBase, RasPiDrive> {
public:
	explicit RasPiDrive(Drivetrain* drive, bool isAutonomous = false);
	
	void Execute() override;

	void End(bool interrupted) override;

	bool IsFinished() override;

private:
	Drivetrain* drivetrain;

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
};