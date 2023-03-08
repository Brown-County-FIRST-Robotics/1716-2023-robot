#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/motorcontrol/MotorControllerGroup.h>
#include <frc/DoubleSolenoid.h>
#include <rev/CANSparkMax.h>
#include <frc/drive/MecanumDrive.h>
#include <frc/TimedRobot.h> //units::scalar

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
	rev::CANSparkMax frontLeft{1, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax frontRight{2, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backLeft{3, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax backRight{4, rev::CANSparkMax::MotorType::kBrushless};

	frc::MecanumDrive robotDrive{frontLeft, backLeft, frontRight, backRight};

};