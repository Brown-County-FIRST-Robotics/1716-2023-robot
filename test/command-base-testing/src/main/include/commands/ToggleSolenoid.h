#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/SolenoidSubsystem.h"

class ToggleSolenoid : public frc2::CommandHelper<frc2::CommandBase, ToggleSolenoid> {
public:
	explicit ToggleSolenoid(SolenoidSubsystem* subsystem);

	void Initialize() override;
	
		void End(bool interrupted) override;
		
private:
	SolenoidSubsystem* solenoidSubsystem;
	frc::DoubleSolenoid::Value currentPosition;
};