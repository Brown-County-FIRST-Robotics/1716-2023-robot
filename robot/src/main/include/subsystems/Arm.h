#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxRelativeEncoder.h>
#include <frc/TimedRobot.h> //units::scalar
#include <frc/DigitalInput.h>

#include "Constants.h"

class Arm : public frc2::SubsystemBase
{
public:
	Arm();

	bool SetToAngle(double elbow_ang, double shoulder_ang);
	bool Arm::ZeroArm();

private:
	rev::CANSparkMax shoulder{ArmConst::SHOULDERID, rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax elbow{ArmConst::ELBOWID, rev::CANSparkMax::MotorType::kBrushless};

	rev::SparkMaxRelativeEncoder elbowEncoder;
	rev::SparkMaxRelativeEncoder shoulderEncoder;

	frc::DigitalInput elbowSwitch{ArmConst::ELBOWSWITCHID};
	frc::DigitalInput shoulerSwitch{ArmConst::SHOULDERSWITCHID};
};