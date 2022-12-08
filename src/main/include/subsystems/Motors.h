#pragma once

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <frc/AnalogOutput.h>

class Motors : public frc2::SubsystemBase {
 public:
  //Method declarations go here
  Motors(); //Constructor

  //void Periodic() override; //there are built in methods such as this, which is called once per frame

  void SetTfx(double speed); //Declare your own methods like so
  void SetTsrx(double speed);

  private:
    //subsystem components such as motors
    WPI_TalonFX tfx;
    WPI_TalonSRX tsrx;
};
