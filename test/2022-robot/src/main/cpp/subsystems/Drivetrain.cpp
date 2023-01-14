#include "Constants.h"
#include "subsystems/Drivetrain.h"

Drivetrain::Drivetrain() {}

void Drivetrain::Drive(double x, double y, double z) {
	robotDrive.DriveCartesian(x, y, z);
}