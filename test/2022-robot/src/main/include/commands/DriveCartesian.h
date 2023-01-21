#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Drivetrain.h"

class DriveCartesian : public frc2::CommandHelper<frc2::CommandBase, DriveCartesian> {
public:
	explicit DriveCartesian(Drivetrain* subsystem, std::function<double()> forward, std::function<double()> right, std::function<double()> rotation);

	void Execute() override;
	
	void End(bool interrupted) override;

private:
	Drivetrain* drivetrain;
	std::function<double()> x;
	std::function<double()> y;
	std::function<double()> z;

	double xSquare;
	double ySquare;
	double zSquare;
};