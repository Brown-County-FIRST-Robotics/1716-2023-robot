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
		arm->StopShoulder();//AddToShoulderGoal(0);
		shoulderStopped=true;
	}
	if (fabs(elbow()) > .05){
		arm->AddToElbowGoal(-elbow() * ArmConst::ELBOW_JOYSTICK_SPEED);
		elbowStopped=false;
	}else if(!elbowStopped){
		arm->StopElbow();//AddToElbowGoal(0);
		elbowStopped=true;
	}
	//std::cout << "Goal:" << arm->GetShoulderGoal() << std::endl;
}

void ArmTeleopControl::End(bool interrupted) {
	arm->StopShoulder();
	arm->StopElbow();
}
