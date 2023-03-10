#define _USE_MATH_DEFINES

#include "subsystems/Arm.h"
#include <math.h>

Arm::Arm(frc::PneumaticHub& hubRef) : hub{hubRef} {
	shoulder.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
	// shoulder.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kForward, false);
	// shoulder.EnableSoftLimit(rev::CANSparkMax::SoftLimitDirection::kReverse, false);
}

void Arm::Periodic() {
	if (directionTicks == 0)
		armDirection.Set(frc::DoubleSolenoid::Value::kOff);
	else if (directionTicks > -1)
		directionTicks--;

	if (brakeTicks == 0)
		armBrake.Set(frc::DoubleSolenoid::Value::kOff);
	else if (brakeTicks > -1)
		brakeTicks--;

	if (clawTicks == 0)
		claw.Set(frc::DoubleSolenoid::Value::kOff);
	else if (clawTicks > -1)
		clawTicks--;
}

// void Arm::SetShoulderLimit(rev::CANSparkMax::SoftLimitDirection direction, double position) {
// 	shoulder.SetSoftLimit(direction, position);
// }

void Arm::SetShoulder(double speed) {
	shoulder.Set(speed * ArmConst::SHOULDER_SPEED);
}

void Arm::ToggleArmDirection() {
	if (directionPos == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		armDirection.Set(frc::DoubleSolenoid::Value::kForward);
		directionPos = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		armDirection.Set(frc::DoubleSolenoid::Value::kReverse);
		directionPos = frc::DoubleSolenoid::Value::kReverse;
	}
	
	directionTicks = SolenoidConst::WAIT_TICKS;
}

void Arm::SetArmDirection(frc::DoubleSolenoid::Value value) {
	armDirection.Set(value);
	directionPos = value;
	
	directionTicks = SolenoidConst::WAIT_TICKS;
}

frc::DoubleSolenoid::Value Arm::GetArmDirection() {
	return directionPos;
}

void Arm::ToggleArmActive() {
	if (brakePos == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		armBrake.Set(frc::DoubleSolenoid::Value::kForward);
		brakePos = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		armBrake.Set(frc::DoubleSolenoid::Value::kReverse);
		brakePos = frc::DoubleSolenoid::Value::kReverse;
	}
	
	brakeTicks = SolenoidConst::WAIT_TICKS;
}

void Arm::SetArmActive(bool active) {
	if (active) {
		armBrake.Set(frc::DoubleSolenoid::Value::kForward);
		brakePos = frc::DoubleSolenoid::Value::kForward;
	}
	else {
		armBrake.Set(frc::DoubleSolenoid::Value::kReverse);
		brakePos = frc::DoubleSolenoid::Value::kReverse;
	}
	
	brakeTicks = SolenoidConst::WAIT_TICKS;
}

frc::DoubleSolenoid::Value Arm::GetArmActive() {
	return brakePos;
}

double Arm::GetArmAngle() {
	return armPotentiometer.Get();
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
