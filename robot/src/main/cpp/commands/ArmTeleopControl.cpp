#include "commands/ArmTeleopControl.h"

#include <utility>

ArmTeleopControl::ArmTeleopControl(Arm* subsystem, std::function<double()> shoulderAxis, 
	std::function<bool()> armUpButton, std::function<bool()> armDownButton) :
	shoulder(std::move(shoulderAxis)), armUp(std::move(armUpButton)), armDown(std::move(armDownButton))
{
	AddRequirements(subsystem);
}

void ArmTeleopControl::Execute() {
	if ((!armUpPressed && armUp())) {
		arm->SetArmDirection(frc::DoubleSolenoid::Value::kForward);
		arm->SetArmActive(true);
		armUpPressed = true;
	}
	else if ((!armDownPressed && armDown())) {
		arm->SetArmDirection(frc::DoubleSolenoid::Value::kReverse);
		arm->SetArmActive(true);
		armDownPressed = true;
	}
	else if ((armUpPressed && !armUp()) || (armDownPressed && !armDown())) {
		arm->SetArmActive(false);
		armUpPressed = false;
		armDownPressed = false;
	}

	arm->SetShoulder(-shoulder() * ArmConst::SHOULDER_SPEED);
}

void ArmTeleopControl::End(bool interrupted) {
	arm->SetArmActive(false);
	arm->SetShoulder(0);
}
