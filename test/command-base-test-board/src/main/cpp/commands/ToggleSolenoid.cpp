#include "commands/ToggleSolenoid.h"
#include <frc/smartdashboard/SmartDashboard.h>

ToggleSolenoid::ToggleSolenoid(SolenoidSubsystem* subsystem) : solenoidSubsystem(subsystem) {
	AddRequirements(subsystem);
	currentPosition = frc::DoubleSolenoid::Value::kReverse; //solenoidSubsystem->GetPosition();
}

void ToggleSolenoid::Initialize() {
	if (currentPosition == frc::DoubleSolenoid::Value::kReverse) {
		solenoidSubsystem->SetPosition(frc::DoubleSolenoid::Value::kForward);
		currentPosition = frc::DoubleSolenoid::Value::kForward;
		frc::SmartDashboard::PutBoolean("Solenoid", true);
	}
	else { //if not reverse, set to reverse
		solenoidSubsystem->SetPosition(frc::DoubleSolenoid::Value::kReverse);
		currentPosition = frc::DoubleSolenoid::Value::kReverse;
		frc::SmartDashboard::PutBoolean("Solenoid", false);
	}
}

void ToggleSolenoid::End(bool interrupted) {
	solenoidSubsystem->SetPosition(frc::DoubleSolenoid::Value::kOff);
}