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
	explicit RasPiDrive(Drivetrain* drive);
	
	void Execute() override;

	void End(bool interrupted) override;

private:
	Drivetrain* drivetrain;

	nt::NetworkTableInstance networkTableInst;
	std::shared_ptr<nt::NetworkTable> driveTable;

	nt::FloatSubscriber x; //x, y, and rotational values
	nt::FloatSubscriber y;
	nt::FloatSubscriber z;
};