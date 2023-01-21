#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Motors.h" //Subsystem requirements

class SpinNeo : public frc2::CommandHelper<frc2::CommandBase, SpinNeo> { //always use CommandHelper
 public:
	explicit SpinNeo(Motors* subsystem);
	explicit SpinNeo(Motors* subsystem, std::function<double()> speed_a);

	void Initialize() override; //Called once at the beginning
	void Execute() override;
	
	void End(bool interrupted) override; //Called once at the end
	
	//void IsFinished() override; tells whether the command has finished, checked once per frame, without it the command never ends
	
 private:
	Motors* motors;
	std::function<double()> speed;
};
