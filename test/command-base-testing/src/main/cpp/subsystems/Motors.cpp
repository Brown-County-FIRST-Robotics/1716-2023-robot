#include "subsystems/Motors.h"
#include <iostream>
#include <rev/CANSparkMax.h>

Motors::Motors() : tfx{0}, sparkMax{31, rev::CANSparkMax::MotorType::kBrushless} {} //constructor

void Motors::SetTfx(double speed) {
	tfx.Set(speed);
}

void Motors::SetTsrx(double speed) {
	sparkMax.Set(speed);
}