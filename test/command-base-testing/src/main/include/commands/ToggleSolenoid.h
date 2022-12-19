#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Solenoid.h"

class ToggleSolenoid : public frc2::CommandHelper<frc2::CommandBase, ToggleSolenoid> {
public:
	explicit ToggleSolenoid(Solenoid* subsystem);

	void Initialize() override;
  
    void End(bool interrupted) override;
  	
private:
	Solenoid* solenoid;
	frc::DoubleSolenoid::Value currentPosition;
};