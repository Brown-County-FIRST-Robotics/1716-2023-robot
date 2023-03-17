#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>
#include <rev/CANSparkMax.h>
#include <frc/AnalogPotentiometer.h>
#include <frc/PneumaticHub.h>

#include "Constants.h"

class Arm : public frc2::SubsystemBase {
public:
	Arm(frc::PneumaticHub& hubRef);

	void Periodic() override;

	void SetShoulder(double speed);

	void SetElbowGoal(double position);
	double GetElbowGoal();

	// double GetArmAngle();

	void ToggleClaw();
	void SetClaw(frc::DoubleSolenoid::Value value);
	frc::DoubleSolenoid::Value GetClaw();

private:
	rev::CANSparkMax shoulder{ArmConst::SHOULDER_ID, rev::CANSparkMaxLowLevel::MotorType::kBrushed};

	rev::CANSparkMax elbow{ArmConst::ELBOW_ID, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
	rev::SparkMaxPIDController elbowPid;
	rev::SparkMaxRelativeEncoder elbowEncoder;
	double elbowPositionGoal = 0;

	frc::PneumaticHub& hub;
	frc::DoubleSolenoid claw = hub.MakeDoubleSolenoid(ArmConst::CLAW_ID[0], ArmConst::CLAW_ID[1]);
	int clawTicks = -1;
	frc::DoubleSolenoid::Value clawPos = frc::DoubleSolenoid::Value::kReverse;

	// frc::AnalogPotentiometer armPotentiometer{ArmConst::ARM_POTENTIOMETER_ID};

	double P = 0.1;
	double I = 0;
	double D = 0;
	double MAX_OUTPUT = 0.4;
};
