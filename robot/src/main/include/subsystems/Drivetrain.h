#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
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

	void ActivateBreakMode(bool doBrakeMode);

	double GetRoll();
	double GetPitch();
	double GetYaw();
	int16_t GetX();
	int16_t GetY();
	int16_t GetZ();

	void SetSolenoidPosition(frc::DoubleSolenoid::Value position);
	frc::DoubleSolenoid::Value GetSolenoidPosition();

private:
	rev::CANSparkMax frontLeft{DrivetrainConst::FRONTLEFTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax frontRight{DrivetrainConst::FRONTRIGHTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backLeft{DrivetrainConst::BACKLEFTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backRight{DrivetrainConst::BACKRIGHTID, rev::CANSparkMax::MotorType::kBrushless};

	frc::MecanumDrive robotDrive{frontLeft, backLeft, frontRight, backRight};

	WPI_Pigeon2 pigeon{DrivetrainConst::PIGEONID};

	frc::PneumaticHub hub{1};
 	frc::DoubleSolenoid solenoid = hub.MakeDoubleSolenoid(0, 1);
	frc::DoubleSolenoid::Value solenoidPos = frc::DoubleSolenoid::Value::kOff;
};