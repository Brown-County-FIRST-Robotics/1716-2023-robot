#pragma once

#include <frc2/command/SubsystemBase.h>
#include <rev/CANSparkMax.h>
#include <rev/SparkMaxRelativeEncoder.h>
#include <frc/DigitalInput.h>

#include "Constants.h"

class Arm : public frc2::SubsystemBase {
public:
	Arm();

	bool SetToPosition(double xGoal, double yGoal);
	void SetMotorsZero();
	double GetEncoder(int motorID);
	bool Zero();
	double getX();
	double getY();

private:
	rev::CANSparkMax shoulder{ArmConst::ID[0], rev::CANSparkMax::MotorType::kBrushless};
	rev::CANSparkMax elbow{ArmConst::ID[1], rev::CANSparkMax::MotorType::kBrushless};

	rev::SparkMaxRelativeEncoder shoulderEncoder;
	rev::SparkMaxRelativeEncoder elbowEncoder;

	frc::DigitalInput shoulderSwitch{ArmConst::LIMIT_SWITCH_ID[0]};
	frc::DigitalInput elbowSwitch{ArmConst::LIMIT_SWITCH_ID[1]};

	double x = 0;
	double y = 0;
};
