#include "subsystems/Drivetrain.h"

Drivetrain::Drivetrain() {
	frontRight.SetInverted(true);
	backRight.SetInverted(true);
}

void Drivetrain::Drive(double x, double y, double z) {
	robotDrive.DriveCartesian(
		CloserToZero(x, xAccelerationCap.Calculate(x)), 
		CloserToZero(y, yAccelerationCap.Calculate(y)), 
		CloserToZero(z, zAccelerationCap.Calculate(z)));
	//robotDrive.DriveCartesian(accelerationCap.Calculate(x), accelerationCap.Calculate(y), accelerationCap.Calculate(z));
}

void Drivetrain::ActivateBreakMode(bool doBrakeMode) {
	if (doBrakeMode) {
		frontLeft.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
		frontRight.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
		backLeft.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
		backRight.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
	}
	else {
		frontLeft.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
		frontRight.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
		backLeft.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
		backRight.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
	}
}

double Drivetrain::CloserToZero(double value1, double value2) {
	if (fabs(value1) < fabs(value2)) {
		return value1;
	}
	else {
		return value2;
	}
}