#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Arm.h"

class ToggleClimberSolenoid : public frc2::CommandHelper<frc2::CommandBase, ToggleClimberSolenoid> {
public:
	explicit ToggleClimberSolenoid(Arm* subsystem);

	void Initialize() override;
	void End(bool interrupted) override;

private:
	Arm* arm;
	frc::DoubleSolenoid::Value currentPosition;
};