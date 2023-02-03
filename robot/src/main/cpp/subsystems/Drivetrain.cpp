#include "subsystems/Drivetrain.h"

Drivetrain::Drivetrain() {
	frontRight.SetInverted(true);
	backRight.SetInverted(true);

	robotDrive.SetSafetyEnabled(false); //delete this later
}

void Drivetrain::Drive(double x, double y, double z) {
	if (solenoidPos == frc::DoubleSolenoid::Value::kReverse) {
		robotDrive.DriveCartesian(x, y, z);
	}
	else { //don't strafe in traction mode
		robotDrive.DriveCartesian(x, 0, z);
	}
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

double Drivetrain::GetRoll() {
	return pigeon.GetRoll();
}

double Drivetrain::GetPitch() {
	return pigeon.GetPitch();
}

double Drivetrain::GetYaw() {
	return pigeon.GetYaw();
}

int16_t Drivetrain::GetX() {
	int16_t accelerometer[3];
	pigeon.GetBiasedAccelerometer(accelerometer);

	return accelerometer[0];
}

int16_t Drivetrain::GetY() {
	int16_t accelerometer[3];
	pigeon.GetBiasedAccelerometer(accelerometer);

	return accelerometer[1];
}

int16_t Drivetrain::GetZ() {
	int16_t accelerometer[3];
	pigeon.GetBiasedAccelerometer(accelerometer);

	return accelerometer[2];
}

void Drivetrain::SetSolenoidPosition(frc::DoubleSolenoid::Value position) {
	solenoid.Set(position);
	solenoidPos = position; //cache for efficiency
}

frc::DoubleSolenoid::Value Drivetrain::GetSolenoidPosition() {
	return solenoidPos;
}