#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include <utility> //contains lambda expressions (I think...)

#include "subsystems/Arm.h"

class Shoot : public frc2::CommandHelper<frc2::CommandBase, Shoot> {
 public:
  explicit Shoot(Arm* subsystem, std::function<double()> speed_a);
  
    void Execute() override;

    void End(bool interrupted) override;
  
    //bool IsFinished() override;	
 private:
  Arm* arm;
  std::function<double()> speed; //lambda function, allows for constant updating of shooter speed
};