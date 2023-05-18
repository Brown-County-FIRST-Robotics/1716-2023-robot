#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <frc/drive/MecanumDrive.h>
#include <frc/filter/SlewRateLimiter.h>

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

	void AddToMaxSpeed(float amountToAdd);

private:
	rev::CANSparkMax frontLeft{DrivetrainConst::FRONTLEFTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax frontRight{DrivetrainConst::FRONTRIGHTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backLeft{DrivetrainConst::BACKLEFTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backRight{DrivetrainConst::BACKRIGHTID, rev::CANSparkMax::MotorType::kBrushless};

	frc::MecanumDrive robotDrive{frontLeft, backLeft, frontRight, backRight};

	float maxSpeed = 0.3;
};