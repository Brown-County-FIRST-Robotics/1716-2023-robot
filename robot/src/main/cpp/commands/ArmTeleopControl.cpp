#include "commands/ArmTeleopControl.h"

#include <utility>

ArmTeleopControl::ArmTeleopControl(Arm* subsystem, std::function<double()> shoulderAxis, 
	std::function<double()> elbowAxis, std::function<bool()> clawButton) : arm(subsystem),
	shoulder(std::move(shoulderAxis)), elbow(std::move(elbowAxis)), claw(std::move(clawButton))
{
	AddRequirements(subsystem);
}

void ArmTeleopControl::Execute() {

	if (!clawPressed && claw()) {
		arm->ToggleClaw();
		clawPressed = true;
	}
	else if (!claw())
		clawPressed = false;

	arm->SetShoulder(-shoulder());
	// arm->SetElbow(-elbow());
}

void ArmTeleopControl::End(bool interrupted) {
	arm->SetShoulder(0);
	// arm->SetElbow(0);
}
