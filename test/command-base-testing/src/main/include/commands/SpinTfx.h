#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Motors.h" //Subsystem requirements

class SpinTfx : public frc2::CommandHelper<frc2::CommandBase, SpinTfx> { //always use CommandHelper
 public:
  explicit SpinTfx(Motors* subsystem);

  void Initialize() override; //Called once at the beginning
  
  void End(bool interrupted) override; //Called once at the end
  
  //void IsFinished() override; tells whether the command has finished, checked once per frame, without it the command never ends
	
 private:
  Motors* motors;
};
