#include "commands/AutoSwitchDrive.h"

#include <utility>

AutoSwitchDrive::AutoSwitchDrive(Drivetrain* subsystem, std::function<double()> forward, std::function<double()> right, std::function<double()> rotation, std::function<bool()> brake) 
	: drivetrain(subsystem), x(std::move(forward)), y(std::move(right)), z(std::move(rotation)), doBrake(std::move(brake))
{
	AddRequirements(subsystem);
}

void AutoSwitchDrive::Execute() {
	xSquare = x() * fabs(x()); //multiply each axis by its absolute value, this makes acceleration exponential rather than linear
	ySquare = y() * fabs(y());
	zSquare = z() * fabs(z());

	drivetrain->Drive(
		-CloserToZero(xSquare, xAccelerationCap.Calculate(xSquare)), //x must be inverted
		CloserToZero(ySquare, yAccelerationCap.Calculate(ySquare)), 
		CloserToZero(zSquare, zAccelerationCap.Calculate(zSquare)));

	UpdateBrake(doBrake());

	if (x() != 0 && y() != 0)
		leftStickAngle = (atan2(y(), x()) * (180 / M_PI)) + 180;
	else
		leftStickAngle = -1;

	if ((((leftStickAngle > 45 && leftStickAngle < 135) || (leftStickAngle > 225 && leftStickAngle < 315)) && leftStickAngle != -1)
		&& drivetrain->GetSolenoid() == frc::DoubleSolenoid::Value::kReverse) 
		drivetrain->SetSolenoid(frc::DoubleSolenoid::Value::kForward);
	else if (((leftStickAngle < 45 || leftStickAngle > 315 || (leftStickAngle > 135 && leftStickAngle < 225)) && leftStickAngle != -1)
		&& drivetrain->GetSolenoid() == frc::DoubleSolenoid::Value::kForward) 
		drivetrain->SetSolenoid(frc::DoubleSolenoid::Value::kReverse);
}

void AutoSwitchDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}

void AutoSwitchDrive::UpdateBrake(bool brake) {
	if (brake) {
		drivetrain->ActivateBreakMode(true);
	}
	else {
		drivetrain->ActivateBreakMode(false);
	}
}

double AutoSwitchDrive::CloserToZero(double value1, double value2) {
	if (fabs(value1) < fabs(value2)) {
		return value1;
	}
	else {
		return value2;
	}
}