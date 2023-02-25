#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>

#include "Constants.h"

class Arm : public frc2::SubsystemBase {
public:
	Arm();

	void Periodic() override;

	void ToggleDirection();
	void SetDirection(frc::DoubleSolenoid::Value value);
	frc::DoubleSolenoid::Value GetDirection();

	void ToggleUpperArmActive();
	void SetUpperArmActive(frc::DoubleSolenoid::Value value);
	frc::DoubleSolenoid::Value GetUpperArmActive();

	void ToggleForearmActive();
	void SetForearmActive(frc::DoubleSolenoid::Value value);
	frc::DoubleSolenoid::Value GetForearmActive();

private:
	frc::DoubleSolenoid upperArmBrake = SolenoidConst::hub.MakeDoubleSolenoid(ArmConst::UPPER_ARM_BRAKE_ID[0], ArmConst::UPPER_ARM_BRAKE_ID[1]);
	frc::DoubleSolenoid upperArmDirection = SolenoidConst::hub.MakeDoubleSolenoid(ArmConst::UPPER_ARM_DIRECTION_ID[0], ArmConst::UPPER_ARM_DIRECTION_ID[1]);
	frc::DoubleSolenoid forearmBrake = SolenoidConst::hub.MakeDoubleSolenoid(ArmConst::FOREARM_BRAKE_ID[0], ArmConst::FOREARM_BRAKE_ID[1]);
	frc::DoubleSolenoid forearmDirection = SolenoidConst::hub.MakeDoubleSolenoid(ArmConst::FOREARM_DIRECTION_ID[0], ArmConst::FOREARM_DIRECTION_ID[1]);
	frc::DoubleSolenoid claw = SolenoidConst::hub.MakeDoubleSolenoid(ArmConst::CLAW_ID[0], ArmConst::CLAW_ID[1]);

	int directionTicks = -1;
	int upperArmBrakeTicks = -1;
	int forearmBrakeTicks = -1;
	int clawTicks = -1;

	frc::DoubleSolenoid::Value direction = frc::DoubleSolenoid::Value::kReverse;
	frc::DoubleSolenoid::Value upperArmActive = frc::DoubleSolenoid::Value::kReverse; //not active
	frc::DoubleSolenoid::Value forearmActive = frc::DoubleSolenoid::Value::kReverse;
};
