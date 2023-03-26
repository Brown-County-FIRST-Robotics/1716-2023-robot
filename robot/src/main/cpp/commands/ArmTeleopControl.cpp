#include "commands/ArmTeleopControl.h"

#include <utility>
#include <iostream>

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


	if (fabs(shoulder()) > .15){
		arm->AddToShoulderGoal(-shoulder() * ArmConst::SHOULDER_JOYSTICK_SPEED);
		shoulderStopped=false;
	}else if (!shoulderStopped){ 
		// when you let go of joystick, stop the arm, but only the first time. 
		//After the first time, stop applying updates, or the arm can lock in new positions while drifting
		arm->StopShoulder();
		shoulderStopped=true;
	}
	if (fabs(elbow()) > .05){
		arm->AddToElbowGoal(-elbow() * ArmConst::ELBOW_JOYSTICK_SPEED);
		elbowStopped=false;
	}else if(!elbowStopped){
		// when you let go of joystick, stop the arm, but only the first time. 
		//After the first time, stop applying updates, or the arm can lock in new positions while drifting
		arm->StopElbow();
		elbowStopped=true;
	}
}

void ArmTeleopControl::End(bool interrupted) {
	arm->StopShoulder();
	arm->StopElbow();
}
