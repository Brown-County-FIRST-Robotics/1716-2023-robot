#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxRelativeEncoder.h>
#include <frc/drive/MecanumDrive.h>
#include <frc/filter/SlewRateLimiter.h>
#include <frc/TimedRobot.h> //units::scalar
#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>
#include <frc/PneumaticHub.h>
#include <networktables/FloatTopic.h>
#include <networktables/BooleanTopic.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

#include "Constants.h"

class Drivetrain : public frc2::SubsystemBase {
public:
	Drivetrain();
	
	/**
	* Mecanum cartesian style driving for the Drivetrain.
	* @param x: forwards/backwards
	* @param y: left/right
	* @param z: rotation
	*/
	void Drive(double x, double y, double z);

	void Periodic() override;

	void ActivateBreakMode(bool doBrakeMode);

	double GetRoll();
	double GetPitch();
	double GetYaw();
	int16_t GetX();
	int16_t GetY();
	int16_t GetZ();

	void ToggleSolenoid();
	void SetSolenoid(frc::DoubleSolenoid::Value position);
	frc::DoubleSolenoid::Value GetSolenoid();

	double GetEncoder(int motorID);
	void ResetEncoders();

private:
	rev::CANSparkMax frontLeft{DrivetrainConst::FRONT_LEFT_ID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax frontRight{DrivetrainConst::FRONT_RIGHT_ID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backLeft{DrivetrainConst::BACK_LEFT_ID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backRight{DrivetrainConst::BACK_RIGHT_ID, rev::CANSparkMax::MotorType::kBrushless};

	rev::SparkMaxRelativeEncoder frontLeftEncoder;
	rev::SparkMaxRelativeEncoder frontRightEncoder;
	rev::SparkMaxRelativeEncoder backLeftEncoder;
	rev::SparkMaxRelativeEncoder backRightEncoder;

	frc::MecanumDrive robotDrive{frontLeft, backLeft, frontRight, backRight};

	WPI_Pigeon2 pigeon{DrivetrainConst::PIGEON_ID};

	frc::PneumaticHub hub{DrivetrainConst::HUB_ID};
 	frc::DoubleSolenoid flSolenoid = hub.MakeDoubleSolenoid(DrivetrainConst::FL_SOLENOID_ID[0], DrivetrainConst::FL_SOLENOID_ID[1]);
 	frc::DoubleSolenoid frSolenoid = hub.MakeDoubleSolenoid(DrivetrainConst::FR_SOLENOID_ID[0], DrivetrainConst::FR_SOLENOID_ID[1]);
 	frc::DoubleSolenoid blSolenoid = hub.MakeDoubleSolenoid(DrivetrainConst::BL_SOLENOID_ID[0], DrivetrainConst::BL_SOLENOID_ID[1]);
 	frc::DoubleSolenoid brSolenoid = hub.MakeDoubleSolenoid(DrivetrainConst::BR_SOLENOID_ID[0], DrivetrainConst::BR_SOLENOID_ID[1]);
	frc::DoubleSolenoid::Value solenoidPos = frc::DoubleSolenoid::Value::kOff;

	int waitTicksNeeded = -1;

	//Networktables:
	nt::NetworkTableInstance networkTableInst;

	std::shared_ptr<nt::NetworkTable> driveTable;
	std::shared_ptr<nt::NetworkTable> encoderTable;
	std::shared_ptr<nt::NetworkTable> pigeonTable;

	nt::FloatPublisher flEncoder;
	nt::FloatPublisher blEncoder;
	nt::FloatPublisher frEncoder;
	nt::FloatPublisher brEncoder;
	nt::BooleanEntry resetEncodersEntry;

	nt::FloatPublisher yaw;
	nt::FloatPublisher xAccel;
	nt::FloatPublisher yAccel;
};