#include "commands/DriveCartesian.h"

#include <utility>

DriveCartesian::DriveCartesian(Drivetrain* subsystem, 
	std::function<double()> forward, std::function<double()> right, std::function<double()> rotation, 
	std::function<bool()> brakeButton, std::function<bool()> coastButton, std::function<int()> maxSpeedPov) 
	: drivetrain(subsystem), x(std::move(forward)), y(std::move(right)), z(std::move(rotation)),
	doBrake(std::move(brakeButton)), doCoast(std::move(coastButton)), maxSpeedPOV(std::move(maxSpeedPov))
{
	AddRequirements(subsystem);
}

void DriveCartesian::Execute() {
	xSquare = x() * fabs(x()); //multiply each axis by its absolute value, this makes acceleration exponential rather than linear
	ySquare = y() * fabs(y());
	zSquare = z() * fabs(z());

	if (fabs(x()) > DrivetrainConst::JOYSTICKDEADZONE || fabs(y()) > DrivetrainConst::JOYSTICKDEADZONE || fabs(z()) > DrivetrainConst::JOYSTICKDEADZONE)
		drivetrain->Drive(
			CloserToZero(xSquare, xAccelerationCap.Calculate(xSquare)),
			CloserToZero(ySquare, yAccelerationCap.Calculate(ySquare)), 
			CloserToZero(zSquare, zAccelerationCap.Calculate(zSquare)));
	else
		drivetrain->Drive(0, 0, 0);

	UpdateBrake();

	//max speed:
	if (maxSpeedPOV() == 0 && maxSpeedPOVPrevState != 0) {
		drivetrain->AddToMaxSpeed(0.1f);
	}
	if (maxSpeedPOV() == 180 && maxSpeedPOVPrevState != 180) {
		drivetrain->AddToMaxSpeed(-0.1f);
	}
	maxSpeedPOVPrevState = maxSpeedPOV();
}

void DriveCartesian::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}

void DriveCartesian::UpdateBrake() {
	if (doBrake() && !doBrakePrevState) {
		drivetrain->ActivateBreakMode(true);
		doBrakePrevState = true;
	}
	else if (!doBrake() && doBrakePrevState)
		doBrakePrevState = false;
	else if (doCoast() && !doCoastPrevState) {
		drivetrain->ActivateBreakMode(false);
		doCoastPrevState = true;
	}
	else if (!doCoast() && doCoastPrevState) {
		doCoastPrevState = false;
	}
}

double DriveCartesian::CloserToZero(double value1, double value2) {
	if (fabs(value1) < fabs(value2)) {
		return value1;
	}
	else {
		return value2;
	}
}