#include "subsystems/Drivetrain.h"

Drivetrain::Drivetrain() {
	frontRight.SetInverted(true);
	backRight.SetInverted(true);

	robotDrive.SetSafetyEnabled(false); //delete this later

	solenoidPos = solenoid0.Get();
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

void Drivetrain::Periodic() {
	if (waitTicksNeeded == 0) {
		solenoid0.Set(frc::DoubleSolenoid::Value::kOff);
		solenoid1.Set(frc::DoubleSolenoid::Value::kOff);
		solenoid2.Set(frc::DoubleSolenoid::Value::kOff);
		solenoid3.Set(frc::DoubleSolenoid::Value::kOff);
	}
	waitTicksNeeded--;
}

void Drivetrain::ToggleSolenoid() {
	if (solenoidPos == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		solenoid0.Set(frc::DoubleSolenoid::Value::kForward);
		solenoid1.Set(frc::DoubleSolenoid::Value::kForward);
		solenoid2.Set(frc::DoubleSolenoid::Value::kForward);
		solenoid3.Set(frc::DoubleSolenoid::Value::kForward);
		solenoidPos = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		solenoid0.Set(frc::DoubleSolenoid::Value::kReverse);
		solenoid1.Set(frc::DoubleSolenoid::Value::kReverse);
		solenoid2.Set(frc::DoubleSolenoid::Value::kReverse);
		solenoid3.Set(frc::DoubleSolenoid::Value::kReverse);
		solenoidPos = frc::DoubleSolenoid::Value::kReverse;
	}
	
	waitTicksNeeded = DrivetrainConst::WAITTICKS;
}