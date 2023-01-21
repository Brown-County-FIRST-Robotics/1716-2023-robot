#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Drivetrain.h"

class AutoBalance : public frc2::CommandHelper<frc2::CommandBase, AutoBalance> {
public:
	explicit AutoBalance(Drivetrain* subsystem);

	void Execute() override;
	
	void End(bool interrupted) override;

private:
	Drivetrain* drivetrain;

	double pitch;
	double proportional;
	double integral;
	double derivative;

	double integralElement = 0;
	double prevPos = 0;
};