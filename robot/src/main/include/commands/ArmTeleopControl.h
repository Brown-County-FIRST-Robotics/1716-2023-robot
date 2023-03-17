#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Arm.h"

class ArmTeleopControl : public frc2::CommandHelper<frc2::CommandBase, ArmTeleopControl> {
public:
	explicit ArmTeleopControl(Arm* subsystem, 
		std::function<double()> shoulderAxis, 
		std::function<double()> elbowAxis,
		std::function<bool()> clawButton);

	void Execute() override;
	
	void End(bool interrupted) override;

private:
	Arm* arm;

	std::function<double()> shoulder;
	std::function<double()> elbow;
	std::function<bool()> claw;

	bool clawPressed = false;
};