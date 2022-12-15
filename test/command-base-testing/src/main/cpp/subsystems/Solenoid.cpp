#include "subsystems/Solenoid.h"

Solenoid::Solenoid() : piston{frc::PneumaticsModuleType::CTREPCM, 1, 2} {}

void Solenoid::SetPosition(frc::DoubleSolenoid::Value position) {
	piston.Set(position);
}

frc::DoubleSolenoid::Value Solenoid::GetPosition() {
	return piston.Get();
}