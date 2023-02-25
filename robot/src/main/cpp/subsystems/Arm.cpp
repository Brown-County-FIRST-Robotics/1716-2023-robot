#define _USE_MATH_DEFINES

#include "subsystems/Arm.h"
#include <math.h>

Arm::Arm() {}

void Arm::Periodic() {
	if (directionTicks == 0) {
		upperArmDirection.Set(frc::DoubleSolenoid::Value::kOff);
		forearmDirection.Set(frc::DoubleSolenoid::Value::kOff);
	}
	directionTicks--;

	if (upperArmBrakeTicks == 0) {
		upperArmBrake.Set(frc::DoubleSolenoid::Value::kOff);
	}
	upperArmBrakeTicks--;
	
	if (forearmBrakeTicks == 0) {
		forearmBrake.Set(frc::DoubleSolenoid::Value::kOff);
	}
	forearmBrakeTicks--;

	if (clawTicks == 0) {
		claw.Set(frc::DoubleSolenoid::Value::kOff);
	}
	clawTicks--;
}

void Arm::ToggleDirection() {
	if (direction == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		upperArmDirection.Set(frc::DoubleSolenoid::Value::kForward);
		forearmDirection.Set(frc::DoubleSolenoid::Value::kForward);
		direction = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		upperArmDirection.Set(frc::DoubleSolenoid::Value::kReverse);
		forearmDirection.Set(frc::DoubleSolenoid::Value::kReverse);
		direction = frc::DoubleSolenoid::Value::kReverse;
	}
	
	directionTicks = SolenoidConst::WAIT_TICKS;
}

void Arm::SetDirection(frc::DoubleSolenoid::Value value) {
	upperArmDirection.Set(value);
	forearmDirection.Set(value);
	direction = value;
	
	directionTicks = SolenoidConst::WAIT_TICKS;
}

frc::DoubleSolenoid::Value Arm::GetDirection() {
	return direction;
}

void Arm::ToggleUpperArmActive() {
	if (upperArmActive == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		upperArmBrake.Set(frc::DoubleSolenoid::Value::kForward);
		upperArmActive = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		upperArmBrake.Set(frc::DoubleSolenoid::Value::kReverse);
		upperArmActive = frc::DoubleSolenoid::Value::kReverse;
	}
	
	upperArmBrakeTicks = SolenoidConst::WAIT_TICKS;
}

void Arm::SetUpperArmActive(frc::DoubleSolenoid::Value value) {
	upperArmBrake.Set(value);
	upperArmActive = value;
	
	upperArmBrakeTicks = SolenoidConst::WAIT_TICKS;
}

frc::DoubleSolenoid::Value Arm::GetUpperArmActive() {
	return upperArmActive;
}

void Arm::ToggleForearmActive() {
	if (forearmActive == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		forearmBrake.Set(frc::DoubleSolenoid::Value::kForward);
		forearmActive = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		forearmBrake.Set(frc::DoubleSolenoid::Value::kReverse);
		forearmActive = frc::DoubleSolenoid::Value::kReverse;
	}
	
	forearmBrakeTicks = SolenoidConst::WAIT_TICKS;
}

void Arm::SetForearmActive(frc::DoubleSolenoid::Value value) {
	forearmBrake.Set(value);
	forearmActive = value;
	
	forearmBrakeTicks = SolenoidConst::WAIT_TICKS;
}

frc::DoubleSolenoid::Value Arm::GetForearmActive() {
	return forearmActive;
}
