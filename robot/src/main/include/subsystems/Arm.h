#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxRelativeEncoder.h>
#include <frc/DigitalInput.h>

#include "Constants.h"

class Arm : public frc2::SubsystemBase {
public:
	Arm();

	bool SetToAngle(double shoulderAngle, double elbowAngle);
	bool SetSpeed(double shoulderSpeed, double elbowSpeed);
	bool ZeroArm();

private:
	rev::CANSparkMax shoulder{ArmConst::ID[0], rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax elbow{ArmConst::ID[1], rev::CANSparkMax::MotorType::kBrushless};

	rev::SparkMaxRelativeEncoder shoulderEncoder;
	rev::SparkMaxRelativeEncoder elbowEncoder;

	frc::DigitalInput shoulerSwitch{ArmConst::LIMIT_SWITCH_ID[0]};
	frc::DigitalInput elbowSwitch{ArmConst::LIMIT_SWITCH_ID[1]};
};