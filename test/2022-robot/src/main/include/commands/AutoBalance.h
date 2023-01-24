#pragma once

#include <frc2/command/PIDCommand.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Drivetrain.h"

class AutoBalance : public frc2::CommandHelper<frc2::PIDCommand, AutoBalance> {
public:
	explicit AutoBalance(Drivetrain* drive);
	
	void End(bool interrupted) override;

private:
	Drivetrain* drivetrain;
};