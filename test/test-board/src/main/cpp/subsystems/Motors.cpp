#include "subsystems/Motors.h"
#include <iostream>

Motors::Motors() : tfx{0}, tsrx{1}, neo{31, rev::CANSparkMax::MotorType::kBrushless} {} //constructor

void Motors::SetTfx(double speed) {
	tfx.Set(speed);
}

void Motors::SetTsrx(double speed) {
	tsrx.Set(speed);
}

void Motors::SetNeo(double speed){
	neo.Set(speed);
}