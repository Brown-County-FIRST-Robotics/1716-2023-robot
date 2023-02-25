#include "commands/ArmTeleopControl.h"

#include <utility>

ArmTeleopControl::ArmTeleopControl(Arm* subsystem, std::function<int()> POV) 
	: arm(subsystem), pov(std::move(POV))
{
	AddRequirements(subsystem);
}

void ArmTeleopControl::Execute() {
	switch (pov()) {
		case 0:
			arm->SetToPosition(arm->getX(), arm->getY() + 1);
			break;
		case 45:
			arm->SetToPosition(arm->getX() + 1, arm->getY() + 1);
			break;
		case 90:
			arm->SetToPosition(arm->getX() + 1, arm->getY());
			break;
		case 135:
			arm->SetToPosition(arm->getX() + 1, arm->getY() - 1);
			break;
		case 180:
			arm->SetToPosition(arm->getX(), arm->getY() - 1);
			break;
		case 225:
			arm->SetToPosition(arm->getX() - 1, arm->getY() - 1);
			break;
		case 270:
			arm->SetToPosition(arm->getX() - 1, arm->getY());
			break;
		case 315:
			arm->SetToPosition(arm->getX() - 1, arm->getY() + 1);
			break;
		case -1:
			arm->SetToPosition(arm->getX(), arm->getY());
			break;
	}
}

void ArmTeleopControl::End(bool interrupted) {
	arm->SetMotorsZero();
}