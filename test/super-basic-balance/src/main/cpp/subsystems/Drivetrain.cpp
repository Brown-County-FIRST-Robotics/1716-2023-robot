#include "subsystems/Drivetrain.h"

Drivetrain::Drivetrain() {
	frontRight.SetInverted(true);
	backRight.SetInverted(true);
}

void Drivetrain::Drive(double x, double y, double z) {
	// robotDrive.DriveCartesian(x, y, z);
	robotDrive.DriveCartesian(x, y, z);
}