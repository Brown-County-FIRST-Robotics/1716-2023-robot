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
	rev::CANSparkMax frontLeft{DrivetrainConst::FRONTLEFTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax frontRight{DrivetrainConst::FRONTRIGHTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backLeft{DrivetrainConst::BACKLEFTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backRight{DrivetrainConst::BACKRIGHTID, rev::CANSparkMax::MotorType::kBrushless};

	rev::SparkMaxRelativeEncoder frontLeftEncoder;
	rev::SparkMaxRelativeEncoder frontRightEncoder;
	rev::SparkMaxRelativeEncoder backLeftEncoder;
	rev::SparkMaxRelativeEncoder backRightEncoder;

	frc::MecanumDrive robotDrive{frontLeft, backLeft, frontRight, backRight};

	WPI_Pigeon2 pigeon{DrivetrainConst::PIGEONID};

	frc::PneumaticHub hub{DrivetrainConst::HUBID};
 	frc::DoubleSolenoid solenoid0 = hub.MakeDoubleSolenoid(DrivetrainConst::SOLENOID0FORWARDID, DrivetrainConst::SOLENOID0REVERSEID);
 	frc::DoubleSolenoid solenoid1 = hub.MakeDoubleSolenoid(DrivetrainConst::SOLENOID1FORWARDID, DrivetrainConst::SOLENOID1REVERSEID);
 	frc::DoubleSolenoid solenoid2 = hub.MakeDoubleSolenoid(DrivetrainConst::SOLENOID2FORWARDID, DrivetrainConst::SOLENOID2REVERSEID);
 	frc::DoubleSolenoid solenoid3 = hub.MakeDoubleSolenoid(DrivetrainConst::SOLENOID3FORWARDID, DrivetrainConst::SOLENOID3REVERSEID);
	frc::DoubleSolenoid::Value solenoidPos = frc::DoubleSolenoid::Value::kOff;

	int waitTicksNeeded = -1;
};