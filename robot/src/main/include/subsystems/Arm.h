#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>
#include <rev/CANSparkMax.h>
#include <frc/AnalogPotentiometer.h>

#include "Constants.h"

class Arm : public frc2::SubsystemBase {
public:
	Arm();

	void Periodic() override;

	// void SetShoulderLimit(rev::CANSparkMax::SoftLimitDirection direction, double position);
	void SetShoulder(double speed);

	void ToggleArmDirection();
	void SetArmDirection(frc::DoubleSolenoid::Value value);
	frc::DoubleSolenoid::Value GetArmDirection();

	void ToggleArmActive();
	void SetArmActive(bool active);
	frc::DoubleSolenoid::Value GetArmActive();

	double GetArmAngle();

private:
	rev::CANSparkMax shoulder{ArmConst::SHOULDER_ID, rev::CANSparkMaxLowLevel::MotorType::kBrushless};

	frc::DoubleSolenoid armBrake = SolenoidConst::HUB.MakeDoubleSolenoid(ArmConst::ARM_BRAKE_ID[0], ArmConst::ARM_BRAKE_ID[1]);
	frc::DoubleSolenoid armDirection = SolenoidConst::HUB.MakeDoubleSolenoid(ArmConst::ARM_DIRECTION_ID[0], ArmConst::ARM_DIRECTION_ID[1]);
	frc::DoubleSolenoid claw = SolenoidConst::HUB.MakeDoubleSolenoid(ArmConst::CLAW_ID[0], ArmConst::CLAW_ID[1]);

	int directionTicks = -1;
	int brakeTicks = -1;
	int clawTicks = -1;

	frc::DoubleSolenoid::Value directionPos = frc::DoubleSolenoid::Value::kReverse;
	frc::DoubleSolenoid::Value brakePos = frc::DoubleSolenoid::Value::kReverse;

	frc::AnalogPotentiometer armPotentiometer{ArmConst::ARM_POTENTIOMETER_ID};
};
