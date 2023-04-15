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

	frc::PneumaticHub& hub;
	frc::DoubleSolenoid solenoid = hub.MakeDoubleSolenoid(DrivetrainConst::SOLENOID_ID[0], DrivetrainConst::SOLENOID_ID[1]);
	frc::DoubleSolenoid::Value solenoidPos = frc::DoubleSolenoid::Value::kReverse;
	nt::GenericEntry* solenoidIndicator;

	int waitTicksNeeded = -1;

	//Networktables:
	nt::NetworkTableInstance networkTableInst;

	std::shared_ptr<nt::NetworkTable> driveTable;
	std::shared_ptr<nt::NetworkTable> encoderTable;
	std::shared_ptr<nt::NetworkTable> pigeonTable;
	std::shared_ptr<nt::NetworkTable> motorTable;

	nt::FloatPublisher flEncoder;
	nt::FloatPublisher blEncoder;
	nt::FloatPublisher frEncoder;
	nt::FloatPublisher brEncoder;
	nt::BooleanEntry resetEncodersEntry;

	nt::FloatPublisher yaw;
	nt::FloatPublisher xAccel;
	nt::FloatPublisher yAccel;

	nt::GenericEntry* resetPigeonPos;

       //these are from the tutorial and do not match our robot.  we're getting away with it for now
	frc::Translation2d m_frontLeftLocation{0.381_m, 0.381_m};
	frc::Translation2d m_frontRightLocation{0.381_m, -0.381_m};
	frc::Translation2d m_backLeftLocation{-0.381_m, 0.381_m};
	frc::Translation2d m_backRightLocation{-0.381_m, -0.381_m};

	// Creating my kinematics object using the wheel locations.
	frc::MecanumDriveKinematics m_kinematics{
	m_frontLeftLocation, m_frontRightLocation,
	m_backLeftLocation, m_backRightLocation
	};

	// Creating my odometry object from the kinematics object. Here,
	// our starting pose is 5 meters along the long end of the field and in the
	// center of the field along the short end, facing forward.
	frc::MecanumDrivePoseEstimator odometry{
	m_kinematics,
	frc::Rotation2d(units::degree_t(pigeon.GetYaw())),
	frc::MecanumDriveWheelPositions{
		units::meter_t{frontLeftEncoder.GetPosition()/0.44/42.0},
		units::meter_t{frontRightEncoder.GetPosition()/0.44/42.0},	
		units::meter_t{backLeftEncoder.GetPosition()/0.44/42.0},
		units::meter_t{backRightEncoder.GetPosition()/0.44/42.0}
	},
	frc::Pose2d{0_m, 0_m, 0_rad}};






};