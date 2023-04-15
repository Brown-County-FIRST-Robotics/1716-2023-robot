#include "subsystems/Drivetrain.h"

Drivetrain::Drivetrain(frc::PneumaticHub& hubRef) :
	frontLeftEncoder{frontLeft.GetEncoder()}, 
	frontRightEncoder{frontRight.GetEncoder()}, 
	backLeftEncoder{backLeft.GetEncoder()}, 
	backRightEncoder{backRight.GetEncoder()},
	hub{hubRef}
{
	frontLeft.SetInverted(false);
	frontRight.SetInverted(true);
	backLeft.SetInverted(false);
	backRight.SetInverted(true);

	robotDrive.SetSafetyEnabled(false); //delete this later

	//networktables value updates
	networkTableInst = nt::NetworkTableInstance::GetDefault();

	driveTable = networkTableInst.GetTable("1716Drive");
	encoderTable = networkTableInst.GetTable("1716Encoder");	
	pigeonTable = networkTableInst.GetTable("1716Pigeon");

	motorTable = networkTableInst.GetTable("1716Motors");

	flEncoder = encoderTable->GetFloatTopic("frontLeftEncoder").Publish();
	frEncoder = encoderTable->GetFloatTopic("frontRightEncoder").Publish();
	blEncoder = encoderTable->GetFloatTopic("backLeftEncoder").Publish();
	brEncoder = encoderTable->GetFloatTopic("backRightEncoder").Publish();
	resetEncodersEntry = encoderTable->GetBooleanTopic("resetEncoder").GetEntry(false);
	
	xAccel = pigeonTable->GetFloatTopic("xAccel").Publish();
	yAccel = pigeonTable->GetFloatTopic("yAccel").Publish();
	yaw = pigeonTable->GetFloatTopic("yaw").Publish();

	solenoidIndicator = frc::Shuffleboard::GetTab("Drive")
		.Add("Drive Solenoid", false)
		.WithSize(2, 2)
		.WithProperties({
			{"Color when true", nt::Value::MakeString("Maroon")},
			{"Color when false", nt::Value::MakeString("Cyan")}})
		.GetEntry();

	pigeon.Reset();
	odometry.ResetPosition(frc::Rotation2d(units::degree_t(pigeon.GetYaw())),
	frc::MecanumDriveWheelPositions{
				units::meter_t{frontLeftEncoder.GetPosition()/0.44/42.0},
				units::meter_t{frontRightEncoder.GetPosition()/0.44/42.0},
				units::meter_t{backLeftEncoder.GetPosition()/0.44/42.0},
				units::meter_t{backRightEncoder.GetPosition()/0.44/42.0}
	},
	frc::Pose2d{0_m, 0_m, 0_rad});


	pigeon.ConfigMountPose(ctre::phoenix::sensors::AxisDirection::PositiveX, ctre::phoenix::sensors::AxisDirection::PositiveZ);
	resetPigeonPos = frc::Shuffleboard::GetTab("Debugging")
		.Add("Reset Pigeon Position", false)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.WithSize(2, 2)
		.GetEntry();

	SetSolenoid(frc::DoubleSolenoid::Value::kReverse);
}

void Drivetrain::Periodic() {
	if (waitTicksNeeded == 0) {
		solenoid.Set(frc::DoubleSolenoid::Value::kOff);
	}
	waitTicksNeeded--;

	//Networktables
	if (resetEncodersEntry.Get()) {
		ResetEncoders();
		resetEncodersEntry.Set(false);
	}

	flEncoder.Set(GetEncoder(DrivetrainConst::FRONT_LEFT_ID)/0.44);
	frEncoder.Set(GetEncoder(DrivetrainConst::FRONT_RIGHT_ID)/0.44);
	blEncoder.Set(GetEncoder(DrivetrainConst::BACK_LEFT_ID)/0.44);
	brEncoder.Set(GetEncoder(DrivetrainConst::BACK_RIGHT_ID)/0.44);

	xAccel.Set(resetEncodersEntry.GetAtomic().serverTime);
	yAccel.Set(GetY());
	
	yaw.Set(GetYaw());

	if (resetPigeonPos->GetBoolean(false)) {
		pigeon.Reset();
		odometry.ResetPosition(frc::Rotation2d(units::degree_t(pigeon.GetYaw())),
		frc::MecanumDriveWheelPositions{
				units::meter_t{frontLeftEncoder.GetPosition()/0.44/42.0},
				units::meter_t{frontRightEncoder.GetPosition()/0.44/42.0},
				units::meter_t{backLeftEncoder.GetPosition()/0.44/42.0},
				units::meter_t{backRightEncoder.GetPosition()/0.44/42.0}
			},
			frc::Pose2d{0_m, 0_m, 0_rad});

		resetPigeonPos->SetBoolean(false);
	}
		odometry.Update(
			frc::Rotation2d(units::degree_t(pigeon.GetYaw())),
			frc::MecanumDriveWheelPositions{
				units::meter_t{frontLeftEncoder.GetPosition()/0.44/42.0},
				units::meter_t{frontRightEncoder.GetPosition()/0.44/42.0},
				units::meter_t{backLeftEncoder.GetPosition()/0.44/42.0},
				units::meter_t{backRightEncoder.GetPosition()/0.44/42.0}
			}
		);
	auto pos=FetchPos();
	std::cout << "x:" << pos.X().value() << "\ty:" << pos.Y().value() << "\tr:" << pos.Rotation().Degrees().value()  << '\n';
}

void Drivetrain::Drive(double x, double y, double z, bool headless) { //headless means field-oriented
	motorTable->PutNumber("x", x); //Todo: update this to be consistent with the rest of the system
	motorTable->PutNumber("y", y);
	motorTable->PutNumber("r", z);

	if (solenoidPos == frc::DoubleSolenoid::Value::kReverse) {
		if (!headless)
			robotDrive.DriveCartesian(x * DrivetrainConst::MAX_SPEED, y * DrivetrainConst::MAX_SPEED, z * DrivetrainConst::MAX_SPEED * 0.6);
		else
			robotDrive.DriveCartesian(x * DrivetrainConst::MAX_SPEED, y * DrivetrainConst::MAX_SPEED, z * DrivetrainConst::MAX_SPEED * 0.6, 
				pigeon.GetRotation2d().operator*(-1));
	}
	else { //don't strafe in traction mode
		robotDrive.DriveCartesian(x * DrivetrainConst::MAX_SPEED, 0, z * DrivetrainConst::MAX_SPEED * 0.6);
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

int Drivetrain::GetYaw() {
	if ((int)pigeon.GetYaw() % 360 >= 0) { //make it between 0 and 359
		return (int)pigeon.GetYaw() % 360;
	}
	else {
		return ((int)pigeon.GetYaw() % 360) + 360;
	}
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

void Drivetrain::ToggleSolenoid() {
	if (solenoidPos == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		solenoid.Set(frc::DoubleSolenoid::Value::kForward);
		solenoidIndicator->SetBoolean(true);
		solenoidPos = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		solenoid.Set(frc::DoubleSolenoid::Value::kReverse);
		solenoidIndicator->SetBoolean(false);
		solenoidPos = frc::DoubleSolenoid::Value::kReverse;
	}
	
	waitTicksNeeded = SolenoidConst::WAIT_TICKS;
}

void Drivetrain::SetSolenoid(frc::DoubleSolenoid::Value position) {
	solenoid.Set(position);
	if (position == frc::DoubleSolenoid::Value::kForward)
		solenoidIndicator->SetBoolean(true);
	else
		solenoidIndicator->SetBoolean(false);

	solenoidPos = position;
	
	waitTicksNeeded = SolenoidConst::WAIT_TICKS;
}

frc::DoubleSolenoid::Value Drivetrain::GetSolenoid() {
	return solenoidPos;
}

double Drivetrain::GetEncoder(int motorID) {
	if (motorID == DrivetrainConst::FRONT_LEFT_ID) {
		return frontLeftEncoder.GetPosition() / 42.0;
	}
	else if (motorID == DrivetrainConst::FRONT_RIGHT_ID) {
		return frontRightEncoder.GetPosition() / 42.0;
	}
	else if (motorID == DrivetrainConst::BACK_LEFT_ID) {
		return backLeftEncoder.GetPosition() / 42.0;
	}
	else if (motorID == DrivetrainConst::BACK_RIGHT_ID) {
		return backRightEncoder.GetPosition() / 42.0;
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




frc::Pose2d Drivetrain::FetchPos(){
	return odometry.GetEstimatedPosition();
}