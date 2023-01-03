#include "commands/ToggleClimberSolenoid.h"
#include <frc/smartdashboard/SmartDashboard.h>

ToggleClimberSolenoid::ToggleClimberSolenoid(Arm* subsystem) : arm(subsystem) {
	AddRequirements(subsystem);
	currentPosition = arm->GetClimberSolenoid();
}

void ToggleClimberSolenoid::Initialize() {
	if (currentPosition == frc::DoubleSolenoid::Value::kReverse) {
		arm->SetClimberSolenoid(frc::DoubleSolenoid::Value::kForward);
		currentPosition = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		arm->SetClimberSolenoid(frc::DoubleSolenoid::Value::kReverse);
		currentPosition = frc::DoubleSolenoid::Value::kReverse;
	}
}

void ToggleClimberSolenoid::End(bool interrupted) { //should be called 3 ms after `Initialize()`
	arm->SetClimberSolenoid(frc::DoubleSolenoid::Value::kOff);
}