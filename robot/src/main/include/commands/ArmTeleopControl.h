#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Arm.h"

class ArmTeleopControl : public frc2::CommandHelper<frc2::CommandBase, ArmTeleopControl> {
public:
	explicit ArmTeleopControl(Arm* subsystem, 
		std::function<double()> shoulderAxis, 
		std::function<bool()> armUpButton, 
		std::function<bool()> armDownButton);

	void Execute() override;
	
	void End(bool interrupted) override;

private:
	Arm* arm;

	std::function<double()> shoulder;
	std::function<bool()> armUp;
	std::function<bool()> armDown;

	bool armUpPressed = false;
	bool armDownPressed = false;
};