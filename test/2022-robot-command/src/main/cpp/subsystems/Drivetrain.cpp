#include "subsystems/Drivetrain.h"
#include <iostream>

Drivetrain::Drivetrain() {
	frontRight.SetInverted(true);
	backRight.SetInverted(true);

	robotDrive.SetSafetyEnabled(false); //delete this later
}

void Drivetrain::Drive(double x, double y, double z) {
	robotDrive.DriveCartesian(x, y, z);
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

double Drivetrain::GetPitch() {
	return pigeon.GetPitch();
	std::cout << pigeon.GetPitch();
}