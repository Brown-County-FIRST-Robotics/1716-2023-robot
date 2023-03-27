#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Drivetrain.h"

class TeleopDrive : public frc2::CommandHelper<frc2::CommandBase, TeleopDrive> {
public:
	explicit TeleopDrive(Drivetrain* subsystem, std::function<double()> forwardv, std::function<double()> rotation, 
		std::function<bool()> brake, std::function<bool()> headlessToggle, std::function<bool()> lowSpeedMode);

	void Execute() override;
	
	void End(bool interrupted) override;

private:
	void UpdateBrake(bool brake);
	void UpdateHeadless();

	Drivetrain* drivetrain;
	std::function<double()> x;
	std::function<double()> y;
	std::function<double()> z;

	std::function<bool()> doBrake;
	std::function<bool()> headlessButton;
	std::function<bool()> lowSpeedButton;
	double currentSpeed=1;
	bool headlessPressed = false;
	bool headless = false;

	double xSquare;
	double ySquare;
	double zSquare;

	frc::SlewRateLimiter<units::scalar> xAccelerationCap{DrivetrainConst::ACCELERATION_CAP / 1_s};
	frc::SlewRateLimiter<units::scalar> yAccelerationCap{DrivetrainConst::ACCELERATION_CAP / 1_s};
	frc::SlewRateLimiter<units::scalar> zAccelerationCap{DrivetrainConst::ACCELERATION_CAP / 1_s};

	double CloserToZero(double value1, double value2);
};