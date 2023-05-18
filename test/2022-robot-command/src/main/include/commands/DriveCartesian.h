#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc/TimedRobot.h> //units::scalar

#include "subsystems/Drivetrain.h"

class DriveCartesian : public frc2::CommandHelper<frc2::CommandBase, DriveCartesian> {
public:
	explicit DriveCartesian(Drivetrain* subsystem, 
		std::function<double()> verticalAxis, std::function<double()> horizontalAxis, std::function<double()> rotationAxis, 
		std::function<bool()> brakeButton, std::function<bool()> coastButton, std::function<int()> maxSpeedPov);

	void Execute() override;
	
	void End(bool interrupted) override;

private:
	void UpdateBrake();
	double CloserToZero(double value1, double value2);

	Drivetrain* drivetrain;
	std::function<double()> x;
	std::function<double()> y;
	std::function<double()> z;

	std::function<bool()> doBrake;
	bool doBrakePrevState;
	std::function<bool()> doCoast;
	bool doCoastPrevState;

	double xSquare;
	double ySquare;
	double zSquare;

	frc::SlewRateLimiter<units::scalar> xAccelerationCap{DrivetrainConst::ACCELERATIONCAP / 1_s};
	frc::SlewRateLimiter<units::scalar> yAccelerationCap{DrivetrainConst::ACCELERATIONCAP / 1_s};
	frc::SlewRateLimiter<units::scalar> zAccelerationCap{DrivetrainConst::ACCELERATIONCAP / 1_s};

	std::function<int()> maxSpeedPOV;
	int maxSpeedPOVPrevState = -1;
};