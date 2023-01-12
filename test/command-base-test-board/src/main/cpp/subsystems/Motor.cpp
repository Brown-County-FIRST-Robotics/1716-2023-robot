#include "subsystems/Motor.h"
#include <iostream>
#include <rev/CANSparkMax.h>

Motor::Motor() : sparkMax{31, rev::CANSparkMax::MotorType::kBrushless} {} //constructor


void Motor::Set(double speed) {
	sparkMax.Set(speed);
}