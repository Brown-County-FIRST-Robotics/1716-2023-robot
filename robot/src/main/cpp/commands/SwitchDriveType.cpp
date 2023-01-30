#include "commands/SwitchDriveType.h"

SwitchDriveType::SwitchDriveType(Drivetrain* drive) : drivetrain(drive) {
	AddRequirements(drive);
    currentPosition = drivetrain->GetSolenoidPosition();
}

void SwitchDriveType::Initialize() {
	if (currentPosition == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		drivetrain->SetSolenoidPosition(frc::DoubleSolenoid::Value::kForward);
		currentPosition = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		drivetrain->SetSolenoidPosition(frc::DoubleSolenoid::Value::kReverse);
		currentPosition = frc::DoubleSolenoid::Value::kReverse;
	}
}

void SwitchDriveType::End(bool interrupted) {
	drivetrain->SetSolenoidPosition(frc::DoubleSolenoid::Value::kOff);
}