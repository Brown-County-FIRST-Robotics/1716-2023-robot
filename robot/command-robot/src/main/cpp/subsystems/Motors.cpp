#include "subsystems/Motors.h"
#include <iostream>
#include <rev/CANSparkMax.h>

Motors::Motors() : tfx{1}, tsrx{0} {} //constructor

void Motors::SetTfx(double speed) {
  tfx.Set(speed);
}

void Motors::SetTsrx(double speed) {
  tsrx.Set(speed);
}