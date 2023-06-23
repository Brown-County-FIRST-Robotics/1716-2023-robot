#include <iostream>
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
	secondsightTable = networkTableInst.GetTable("SecondSight")->GetSubTable("Apriltags");
	/*
	This entry has not yet been created in SecondSight
	{x,y,r,x_std,y_std,r_std, timestamp}
	x,y,x_std,y_std: cm
	r,r_std: degrees
	timestamp: seconds
	*/
	aprilEntry = secondsightTable->GetDoubleArrayTopic("field_position").Subscribe({});

	solenoidIndicator = frc::Shuffleboard::GetTab("Teleop")
		.Add("Drive Solenoid", false)
		.WithSize(2, 2)
		.WithProperties({
			{"Color when true", nt::Value::MakeString("Maroon")},
			{"Color when false", nt::Value::MakeString("Cyan")}})
		.GetEntry();

	pigeon.Reset();
	pigeon.ConfigMountPose(ctre::phoenix::sensors::AxisDirection::PositiveX, ctre::phoenix::sensors::AxisDirection::PositiveZ);
	odometry.ResetPosition(frc::Rotation2d(units::degree_t(pigeon.GetYaw())),
	frc::MecanumDriveWheelPositions{
				units::meter_t{frontLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
				units::meter_t{frontRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
				units::meter_t{backLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
				units::meter_t{backRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM}
	},
	DrivetrainConst::INITIAL_POSE);

	frc::Shuffleboard::GetTab("Pre Match").Add("Robot Position", poseSender);
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
	
	if (resetPigeonPos->GetBoolean(false)) {
		pigeon.Reset();
		odometry.ResetPosition(frc::Rotation2d(units::degree_t(pigeon.GetYaw())),
		frc::MecanumDriveWheelPositions{
				units::meter_t{frontLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
				units::meter_t{frontRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
				units::meter_t{backLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
				units::meter_t{backRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM}
			},
			DrivetrainConst::INITIAL_POSE);

		resetPigeonPos->SetBoolean(false);
	}

	odometry.Update(
		frc::Rotation2d(units::degree_t(pigeon.GetYaw())),
		frc::MecanumDriveWheelPositions{
			units::meter_t{frontLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
			units::meter_t{frontRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
			units::meter_t{backLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
			units::meter_t{backRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM}
		}
	);
	auto april=aprilEntry.Get();
	if(april.size()==6){ //  TODO: split this up into multiple entries
		frc::Pose2d pose(april[0] * 0.01_m, april[1] * 0.01_m, frc::Rotation2d(april[2] * 1_deg));
		odometry.AddVisionMeasurement(pose, frc::Timer::GetFPGATimestamp(),{0.03,0.03,5.0});
	}
	auto pos=FetchPos();
	poseSender.SetRobotPose(pos);
}

void Drivetrain::Drive(double x, double y, double z, bool headless) { //headless means field-oriented
	if (solenoidPos == frc::DoubleSolenoid::Value::kReverse) {
		if (!headless)
			robotDrive.DriveCartesian(x * DrivetrainConst::MAX_SPEED, y * DrivetrainConst::MAX_SPEED, z * DrivetrainConst::MAX_SPEED * 0.6);
		else
			robotDrive.DriveCartesian(x * DrivetrainConst::MAX_SPEED, y * DrivetrainConst::MAX_SPEED, z * DrivetrainConst::MAX_SPEED * 0.6, 
				-FetchPos().Rotation());
	}
	else { //don't strafe in traction mode
		robotDrive.DriveCartesian(x * DrivetrainConst::MAX_SPEED, 0, z * DrivetrainConst::MAX_SPEED * 0.6);
	}
}


void Drivetrain::DriveVolts(std::vector<units::volt_t> v){
	std::cout << v[0].value() << "  "<< v[1].value() << "  "<< v[2].value() << "  "<< v[3].value() << "\n";
	frontLeft.SetVoltage(v[0]);
	backRight.SetVoltage(v[3]);
	frontRight.SetVoltage(v[2]);
	backLeft.SetVoltage(v[1]);
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

std::vector<double> Drivetrain::GetEncoder() {
	return {
			frontLeftEncoder.GetPosition() / 42.0,
			frontRightEncoder.GetPosition() / 42.0,
			backLeftEncoder.GetPosition() / 42.0,
			backRightEncoder.GetPosition() / 42.0
		};
}

frc::MecanumDriveWheelSpeeds Drivetrain::GetEncoderSpeeds() {
	return frc::MecanumDriveWheelSpeeds{
		frontLeftEncoder.GetVelocity() * (1/1_min) * 6_in * M_PI * (1/12.75),
		frontRightEncoder.GetVelocity() * (1/1_min) * 6_in * M_PI * (1/12.75),
		backLeftEncoder.GetVelocity() * (1/1_min) * 6_in * M_PI * (1/12.75),
		backRightEncoder.GetVelocity() * (1/1_min) * 6_in * M_PI * (1/12.75) 
	};
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