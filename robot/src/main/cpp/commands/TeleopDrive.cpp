#include "commands/TeleopDrive.h"

#include <utility>

TeleopDrive::TeleopDrive(Drivetrain* drive, std::function<double()> forward, std::function<double()> right, std::function<double()> rotation, 
	std::function<bool()> activateBrake, std::function<bool()> deactivateBrake, std::function<bool()> headlessToggle) 
	: drivetrain(drive), x(std::move(forward)), y(std::move(right)), z(std::move(rotation)), 
	startBrake(std::move(activateBrake)), stopBrake(std::move(deactivateBrake)), headlessButton(std::move(headlessToggle))
{
	AddRequirements(drive);
}

void TeleopDrive::Execute() {
	xSquare = x() * fabs(x()); //multiply each axis by its absolute value, this makes acceleration exponential rather than linear
	ySquare = y() * fabs(y());
	zSquare = z() * fabs(z());

	if (!headless)
		drivetrain->Drive(
			-CloserToZero(xSquare, xAccelerationCap.Calculate(xSquare)), //x must be inverted
			CloserToZero(ySquare, yAccelerationCap.Calculate(ySquare)), 
			CloserToZero(zSquare, zAccelerationCap.Calculate(zSquare)));
	else
		drivetrain->Drive(
			-CloserToZero(xSquare, xAccelerationCap.Calculate(xSquare)), //x must be inverted
			CloserToZero(ySquare, yAccelerationCap.Calculate(ySquare)), 
			CloserToZero(zSquare, zAccelerationCap.Calculate(zSquare)),
			true);

	UpdateBrake(startBrake(), stopBrake());
	UpdateHeadless();
}

void TeleopDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}

void TeleopDrive::UpdateBrake(bool doBrake, bool dontBrake) {
	if (doBrake) {
		drivetrain->ActivateBreakMode(true);
	}
	else if (dontBrake) {
		drivetrain->ActivateBreakMode(false);
	}
}

void TeleopDrive::UpdateHeadless() {
	if (headlessButton() && !headlessPressed) { //if pressed for the first time
		headless = !headless;
		headlessPressed = true;
	}
	else if (!headlessButton() && headlessPressed)
		headlessPressed = false;
}

double TeleopDrive::CloserToZero(double value1, double value2) {
	if (fabs(value1) < fabs(value2)) {
		return value1;
	}
	else {
		return value2;
	}
}