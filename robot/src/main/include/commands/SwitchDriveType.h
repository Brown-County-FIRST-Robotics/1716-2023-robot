#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Drivetrain.h"

class SwitchDriveType : public frc2::CommandHelper<frc2::CommandBase, SwitchDriveType> {
public:
	explicit SwitchDriveType(Drivetrain* drive);
	
	void Execute() override;

	void End(bool interrupted) override;

private:
	Drivetrain* drivetrain;
};