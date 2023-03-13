#include "commands/TeleopDrive.h"

#include <utility>

TeleopDrive::TeleopDrive(Drivetrain* subsystem, Led* led, std::function<double()> forward, std::function<double()> right, std::function<double()> rotation, std::function<bool()> brake) 
	: drivetrain(subsystem), led(led), x(std::move(forward)), y(std::move(right)), z(std::move(rotation)), doBrake(std::move(brake))
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

	UpdateBrake(doBrake());
}

void TeleopDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}

void TeleopDrive::UpdateBrake(bool brake) {
	if (brake) {
		drivetrain->ActivateBreakMode(true);
		led->SetColor(LedConst::BRAKE_COLOR);
	}
	else {
		drivetrain->ActivateBreakMode(false);
		led->SetColor(drivetrain->GetSolenoid()==frc::DoubleSolenoid::Value::kForward?LedConst::MECANUM_COLOR:LedConst::TANK_COLOR); // TODO: add real mecanum and tank solenoid values
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