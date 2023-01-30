#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Drivetrain.h"

class TeleopDrive : public frc2::CommandHelper<frc2::CommandBase, TeleopDrive> {
public:
	explicit TeleopDrive(Drivetrain* subsystem, std::function<double()> forward, std::function<double()> right, std::function<double()> rotation, std::function<bool()> brake);

	void Execute() override;
	
	void End(bool interrupted) override;

private:
	Drivetrain* drivetrain;
	std::function<double()> x;
	std::function<double()> y;
	std::function<double()> z;

	std::function<bool()> doBrake;

	double xSquare;
	double ySquare;
	double zSquare;

	void updateBrake(bool brake);

	frc::SlewRateLimiter<units::scalar> xAccelerationCap{DrivetrainConst::ACCELERATIONCAP / 1_s};
	frc::SlewRateLimiter<units::scalar> yAccelerationCap{DrivetrainConst::ACCELERATIONCAP / 1_s};
	frc::SlewRateLimiter<units::scalar> zAccelerationCap{DrivetrainConst::ACCELERATIONCAP / 1_s};

	double CloserToZero(double value1, double value2);
};