#include "subsystems/Drivetrain.h"

Drivetrain::Drivetrain() :
	frontLeftEncoder{frontLeft.GetEncoder()}, 
	frontRightEncoder{frontRight.GetEncoder()}, 
	backLeftEncoder{backLeft.GetEncoder()}, 
	backRightEncoder{backRight.GetEncoder()} 
{
	frontRight.SetInverted(true);
	backRight.SetInverted(true);

	//robotDrive.SetSafetyEnabled(false); //delete this later

	solenoidPos = frSolenoid.Get();

	//networktables value updates
	networkTableInst = nt::NetworkTableInstance::GetDefault();

	driveTable = networkTableInst.GetTable("1716Drive");
	encoderTable = networkTableInst.GetTable("1716Encoder");	
	pigeonTable = networkTableInst.GetTable("1716Pigeon");

	flEncoder = encoderTable->GetFloatTopic("frontLeftEncoder").Publish();
	frEncoder = encoderTable->GetFloatTopic("frontRightEncoder").Publish();
	blEncoder = encoderTable->GetFloatTopic("backLeftEncoder").Publish();
	brEncoder = encoderTable->GetFloatTopic("backRightEncoder").Publish();
	resetEncodersEntry = encoderTable->GetBooleanTopic("resetEncoder").GetEntry(false);
	
	xAccel = pigeonTable->GetFloatTopic("xAccel").Publish();
	yAccel = pigeonTable->GetFloatTopic("yAccel").Publish();
	yaw = pigeonTable->GetFloatTopic("yaw").Publish();
}

void Drivetrain::Drive(double x, double y, double z) {
	// if (solenoidPos == frc::DoubleSolenoid::Value::kReverse) {
	robotDrive.DriveCartesian(x * DrivetrainConst::MAX_SPEED, y * DrivetrainConst::MAX_SPEED, z * DrivetrainConst::MAX_SPEED);
	// }
	// else { //don't strafe in traction mode
	// 	robotDrive.DriveCartesian(x * DrivetrainConst::MAX_SPEED, 0, z * DrivetrainConst::MAX_SPEED);
	// }
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
		flSolenoid.Set(frc::DoubleSolenoid::Value::kOff);
		frSolenoid.Set(frc::DoubleSolenoid::Value::kOff);
		blSolenoid.Set(frc::DoubleSolenoid::Value::kOff);
		brSolenoid.Set(frc::DoubleSolenoid::Value::kOff);
	}
	waitTicksNeeded--;

	//Networktables
	if (resetEncodersEntry.Get()) {
		ResetEncoders();
		resetEncodersEntry.Set(false);
	}

	flEncoder.Set(GetEncoder(DrivetrainConst::FRONT_LEFT_ID));
	frEncoder.Set(GetEncoder(DrivetrainConst::FRONT_RIGHT_ID));
	blEncoder.Set(GetEncoder(DrivetrainConst::BACK_LEFT_ID));
	brEncoder.Set(GetEncoder(DrivetrainConst::BACK_RIGHT_ID));

	xAccel.Set(GetX());
	yAccel.Set(GetY());
	yaw.Set(GetYaw());
}

void Drivetrain::ToggleSolenoid() {
	if (solenoidPos == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		flSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
		frSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
		blSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
		brSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
		solenoidPos = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		flSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
		frSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
		blSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
		brSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
		solenoidPos = frc::DoubleSolenoid::Value::kReverse;
	}
	
	waitTicksNeeded = DrivetrainConst::WAIT_TICKS;
}

void Drivetrain::SetSolenoid(frc::DoubleSolenoid::Value position) {
	flSolenoid.Set(position);
	frSolenoid.Set(position);
	blSolenoid.Set(position);
	brSolenoid.Set(position);
	solenoidPos = position;
	
	waitTicksNeeded = DrivetrainConst::WAIT_TICKS;
}

frc::DoubleSolenoid::Value Drivetrain::GetSolenoid() {
	return solenoidPos;
}

double Drivetrain::GetEncoder(int motorID) {
	if (motorID == DrivetrainConst::FRONT_LEFT_ID) {
		return frontLeftEncoder.GetPosition()/42.0;
	}
	else if (motorID == DrivetrainConst::FRONT_RIGHT_ID) {
		return frontRightEncoder.GetPosition()/42.0;
	}
	else if (motorID == DrivetrainConst::BACK_LEFT_ID) {
		return backLeftEncoder.GetPosition()/42.0;
	}
	else if (motorID == DrivetrainConst::BACK_RIGHT_ID) {
		return backRightEncoder.GetPosition()/42.0;
	}
	else {
		return 0;
	}
}

void Drivetrain::ResetEncoders() {
	frontLeftEncoder.SetPosition(0);
	frontRightEncoder.SetPosition(0);
	backLeftEncoder.SetPosition(0);
	backRightEncoder.SetPosition(0);
}