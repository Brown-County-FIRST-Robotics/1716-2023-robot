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


	if (fabs(shoulder()) > .05)
		arm->AddToShoulderGoal(-shoulder() * ArmConst::SHOULDER_JOYSTICK_SPEED);
	if (fabs(elbow()) > .05)
		arm->AddToElbowGoal(-elbow() * ArmConst::ELBOW_JOYSTICK_SPEED);
}

void ArmTeleopControl::End(bool interrupted) {
	arm->StopShoulder();
	arm->StopElbow();
}
