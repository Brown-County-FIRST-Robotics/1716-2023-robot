#include "commands/ToggleSolenoid.h"
#include <frc/smartdashboard/SmartDashboard.h>

ToggleSolenoid::ToggleSolenoid(Solenoid* subsystem){
	AddRequirements(subsystem);
	solenoid = new Solenoid();
	currentPosition = frc::DoubleSolenoid::Value::kReverse;
}

void ToggleSolenoid::Initialize() {
	if (currentPosition == frc::DoubleSolenoid::Value::kReverse) {
		solenoid->SetPosition(frc::DoubleSolenoid::Value::kForward);
		currentPosition = frc::DoubleSolenoid::Value::kForward;
		frc::SmartDashboard::PutBoolean("Solenoid", true);
	}
	else { //if not reverse, set to reverse
		solenoid->SetPosition(frc::DoubleSolenoid::Value::kReverse);
		currentPosition = frc::DoubleSolenoid::Value::kReverse;
		frc::SmartDashboard::PutBoolean("Solenoid", false);
	}
}

void ToggleSolenoid::End(bool interrupted) {
	solenoid->SetPosition(frc::DoubleSolenoid::Value::kOff);
}