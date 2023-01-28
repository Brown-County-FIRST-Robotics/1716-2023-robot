#include "subsystems/Neo.h"
#include <iostream>

Neo::Neo() : neo{31, rev::CANSparkMax::MotorType::kBrushless} {} //constructor

void Neo::SetNeo(double speed){
	neo.Set(speed);
}