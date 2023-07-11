#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxRelativeEncoder.h>
#include <frc/drive/MecanumDrive.h>
#include <frc/filter/SlewRateLimiter.h>
#include <frc/TimedRobot.h> //units::scalar
#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>
#include <networktables/FloatTopic.h>
#include <networktables/BooleanTopic.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/PneumaticHub.h>
#include <frc/kinematics/MecanumDriveOdometry.h>
#include <frc/estimator/MecanumDrivePoseEstimator.h>
#include <networktables/DoubleArrayTopic.h>
#include <frc/smartdashboard/Field2d.h>


#include "Constants.h"

class Drivetrain : public frc2::SubsystemBase {
public:
	Drivetrain(frc::PneumaticHub& hubRef);
	
	/**
	* Mecanum cartesian style driving for the Drivetrain.
	* @param x: forwards/backwards
	* @param y: left/right
	* @param z: rotation
	*/
	void Drive(double x, double y, double z, bool headless = false);
	void DriveVolts(std::vector<units::volt_t> z);
	void SetPose(frc::Pose2d pose);


	void Periodic() override;

	void ActivateBreakMode(bool doBrakeMode);
	frc::Pose2d FetchPos();

	double GetRoll();
	double GetPitch();
	int GetYaw();
	int16_t GetX();
	int16_t GetY();
	int16_t GetZ();

	void ToggleSolenoid();
	void SetSolenoid(frc::DoubleSolenoid::Value position);
	frc::DoubleSolenoid::Value GetSolenoid();
	frc::MecanumDriveWheelSpeeds GetEncoderSpeeds();
	std::vector<double> GetEncoder();
	void ResetEncoders();
		frc::MecanumDriveKinematics kinematics{
		DrivetrainConst::WHEEL_POS_FL_MECANUM,
		DrivetrainConst::WHEEL_POS_FR_MECANUM,
		DrivetrainConst::WHEEL_POS_BL_MECANUM,
		DrivetrainConst::WHEEL_POS_BR_MECANUM};

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

	frc::PneumaticHub& hub;
	frc::DoubleSolenoid solenoid = hub.MakeDoubleSolenoid(DrivetrainConst::SOLENOID_ID[0], DrivetrainConst::SOLENOID_ID[1]);
	frc::DoubleSolenoid::Value solenoidPos = frc::DoubleSolenoid::Value::kReverse;
	nt::GenericEntry* solenoidIndicator;

	int waitTicksNeeded = -1;

	//Networktables:
	nt::NetworkTableInstance networkTableInst;
	std::shared_ptr<nt::NetworkTable> secondsightTable;
	nt::DoubleArraySubscriber aprilEntry;

	nt::GenericEntry* resetPigeonPos;

	// Creating kinematics object using the wheel locations.


	// Creating my odometry object from the kinematics object. Here,
	// our starting pose is 5 meters along the long end of the field and in the
	// center of the field along the short end, facing forward.
	frc::MecanumDrivePoseEstimator odometry{
	kinematics,
	frc::Rotation2d(units::degree_t(pigeon.GetYaw())),
	frc::MecanumDriveWheelPositions{
		units::meter_t{frontLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
		units::meter_t{frontRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},	
		units::meter_t{backLeftEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM},
		units::meter_t{backRightEncoder.GetPosition() * DrivetrainConst::WHEEL_EFFECTIVE_DIAMETER_MECANUM}
	},
	DrivetrainConst::INITIAL_POSE};
	frc::Field2d poseSender;


};