#include "commands/ToggleClimberSolenoid.h"
#include <frc/smartdashboard/SmartDashboard.h>

ToggleClimberSolenoid::ToggleClimberSolenoid(Arm* subsystem) : arm(subsystem) {
	AddRequirements(subsystem);
	currentPosition = frc::DoubleSolenoid::Value::kReverse;
	//currentPosition = arm->GetClimberSolenoid();
}

void ToggleClimberSolenoid::Initialize() {
	if (currentPosition == frc::DoubleSolenoid::Value::kReverse) {
		arm->SetClimberSolenoid(frc::DoubleSolenoid::Value::kForward);
		currentPosition = frc::DoubleSolenoid::Value::kForward;
		frc::SmartDashboard::PutBoolean("Climber Solenoid", true);
	}
	else { //if not reverse, set to reverse
		arm->SetClimberSolenoid(frc::DoubleSolenoid::Value::kReverse);
		currentPosition = frc::DoubleSolenoid::Value::kReverse;
		frc::SmartDashboard::PutBoolean("Climber Solenoid", false);
	}
}

void ToggleClimberSolenoid::End(bool interrupted) { //should be called 3 ms after `Initialize()`
	arm->SetClimberSolenoid(frc::DoubleSolenoid::Value::kOff);
}