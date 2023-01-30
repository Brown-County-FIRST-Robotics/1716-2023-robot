#include "commands/TeleopDrive.h"

#include <utility>

TeleopDrive::TeleopDrive(Drivetrain* subsystem, std::function<double()> forward, std::function<double()> right, std::function<double()> rotation, std::function<bool()> brake) 
	: drivetrain(subsystem), x(std::move(forward)), y(std::move(right)), z(std::move(rotation)), doBrake(std::move(brake))
{
	AddRequirements(subsystem);
}

void TeleopDrive::Execute() {
	xSquare = x() * fabs(x()); //multiply each axis by its absolute value, this makes acceleration exponential rather than linear
	ySquare = y() * fabs(y());
	zSquare = z() * fabs(z());

	drivetrain->Drive(
		-CloserToZero(xSquare, xAccelerationCap.Calculate(xSquare)), //x must be inverted
		CloserToZero(ySquare, yAccelerationCap.Calculate(ySquare)), 
		CloserToZero(zSquare, zAccelerationCap.Calculate(zSquare)));

	updateBrake(doBrake());
}

void TeleopDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}

void TeleopDrive::updateBrake(bool brake) {
	if (brake) {
		drivetrain->ActivateBreakMode(true);
	}
	else {
		drivetrain->ActivateBreakMode(false);
	}
}

double TeleopDrive::CloserToZero(double value1, double value2) {
	if (fabs(value1) < fabs(value2)) {
		return value1;
	}
	else {
		return value2;
	}
}