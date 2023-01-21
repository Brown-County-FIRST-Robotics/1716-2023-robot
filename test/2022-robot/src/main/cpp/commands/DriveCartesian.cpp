#include "commands/DriveCartesian.h"

#include <utility>

DriveCartesian::DriveCartesian(Drivetrain* subsystem, std::function<double()> forward, std::function<double()> right, std::function<double()> rotation, std::function<bool()> brake) 
	: drivetrain(subsystem), x(std::move(forward)), y(std::move(right)), z(std::move(rotation)), doBrake(std::move(brake))
{
	AddRequirements(subsystem);
}

void DriveCartesian::Execute() {
	xSquare = x() * fabs(x()); //multiply each axis by its absolute value, this makes acceleration exponential rather than linear
	ySquare = y() * fabs(y());
	zSquare = z() * fabs(z());

	drivetrain->Drive(-xSquare, ySquare, zSquare); //x must be inverted

	updateBrake(doBrake);
}

void DriveCartesian::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}

void DriveCartesian::updateBrake(std::function<bool()> brake) {
	if (brake()) {
		drivetrain->ActivateBreakMode(true);
	}
	else {
		drivetrain->ActivateBreakMode(false);
	}
}