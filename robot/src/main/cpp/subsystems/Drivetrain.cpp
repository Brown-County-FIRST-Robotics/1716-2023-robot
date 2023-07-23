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

	navx.Reset();
	odometry.ResetPosition(frc::Rotation2d(navx.GetAngle()*1_deg),
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
	frontLeft.BurnFlash();
	frontRight.BurnFlash();
	backLeft.BurnFlash();
	backRight.BurnFlash();

}

void Drivetrain::SetPose(frc::Pose2d pose){
	navx.Reset();
	odometry.ResetPosition(frc::Rotation2d(navx.GetAngle()*1_deg),
		frc::MecanumDriveWheelPositions{
				units::meter_t{frontLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
				units::meter_t{frontRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
				units::meter_t{backLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
				units::meter_t{backRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM}
			},
			pose);
}

void Drivetrain::Periodic() {
	if (waitTicksNeeded == 0) {
		solenoid.Set(frc::DoubleSolenoid::Value::kOff);
	}
	waitTicksNeeded--;

	//Networktables
	
	if (resetPigeonPos->GetBoolean(false)) {
		navx.Reset();
		odometry.ResetPosition(frc::Rotation2d(navx.GetAngle()*1_deg),
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
		frc::Rotation2d(navx.GetAngle()*1_deg),
		frc::MecanumDriveWheelPositions{
			units::meter_t{frontLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
			units::meter_t{frontRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
			units::meter_t{backLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
			units::meter_t{backRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM}
		}
	);
	if(secondsightTable->GetNumber("field_x", -1)!=-1){
		frc::Pose2d pose(secondsightTable->GetNumber("field_x",0) * 0.01_m, secondsightTable->GetNumber("field_y",0) * 0.01_m, frc::Rotation2d(secondsightTable->GetNumber("field_ang",0) * 1_deg));
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
				FetchPos().Rotation());
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
	return navx.GetRoll();
}

double Drivetrain::GetPitch() {
	return navx.GetPitch();
}

int Drivetrain::GetYaw() {
	if ((int)navx.GetAngle() % 360 >= 0) { //make it between 0 and 359
		return (int)navx.GetAngle() % 360;
	}
	else {
		return ((int)navx.GetAngle() % 360) + 360;
	}
}

float Drivetrain::GetX() {
	return navx.GetRawAccelX();
}

float Drivetrain::GetY() {
	return navx.GetRawAccelY();
}

float Drivetrain::GetZ() {
	return navx.GetRawAccelZ();
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