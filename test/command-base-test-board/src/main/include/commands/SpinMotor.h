#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Motor.h" //Subsystem requirements

class SpinMotor : public frc2::CommandHelper<frc2::CommandBase, SpinMotor> { //always use CommandHelper
 public:
	explicit SpinMotor(Motor* subsystem, std::function<double()> speed_a);

	void Execute() override; //Called once at the beginning
	
	void End(bool interrupted) override; //Called once at the end
	
	//void IsFinished() override; tells whether the command has finished, checked once per frame, without it the command never ends
	
 private:
	Motor* motor;
	std::function<double()> speed;
};
