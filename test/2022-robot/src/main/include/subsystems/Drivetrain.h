#pragma once

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <frc/motorcontrol/MotorControllerGroup.h>
#include <frc/DoubleSolenoid.h>
#include <rev/CANSparkMax.h>
#include <frc/drive/MecanumDrive.h>
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

private:
	rev::CANSparkMax frontLeft{DrivetrainConstants::FRONTLEFTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backLeft{DrivetrainConstants::BACKLEFTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax frontRight{DrivetrainConstants::FRONTRIGHTID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backRight{DrivetrainConstants::BACKRIGHTID, rev::CANSparkMax::MotorType::kBrushless};

	frc::MecanumDrive robotDrive{frontLeft, backLeft, frontRight, backRight};
};