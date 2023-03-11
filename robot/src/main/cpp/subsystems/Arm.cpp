#define _USE_MATH_DEFINES

#include "subsystems/Arm.h"
#include <math.h>

Arm::Arm(frc::PneumaticHub& hubRef) : hub{hubRef} {
	elbow.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
	// elbow.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, false);
	// elbow.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, false);
}

void Arm::Periodic() {
	if (clawTicks == 0)
		claw.Set(frc::DoubleSolenoid::Value::kOff);
	else if (clawTicks > -1)
		clawTicks--;
}

// void Arm::SetShoulderLimit(rev::CANSparkMax::SoftLimitDirection direction, double position) {
// 	elbow.SetSoftLimit(direction, position);
// }

void Arm::SetShoulder(double speed) {
	shoulder.Set(-speed * ArmConst::SHOULDER_SPEED);
}

void Arm::SetElbow(double speed) {
	elbow.Set(-speed * ArmConst::ELBOW_SPEED);
}

void Arm::ToggleClaw() {
	if (clawPos == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		claw.Set(frc::DoubleSolenoid::Value::kForward);
		clawPos = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		claw.Set(frc::DoubleSolenoid::Value::kReverse);
		clawPos = frc::DoubleSolenoid::Value::kReverse;
	}
	
	clawTicks = SolenoidConst::WAIT_TICKS;
}

void Arm::SetClaw(frc::DoubleSolenoid::Value value) {
	claw.Set(value);
	clawPos = value;
	
	clawTicks = SolenoidConst::WAIT_TICKS;
}

frc::DoubleSolenoid::Value Arm::GetClaw() {
	return clawPos;
}
