#include "subsystems/SolenoidSubsystem.h"

SolenoidSubsystem::SolenoidSubsystem() : piston{frc::PneumaticsModuleType::CTREPCM, 1, 2} {}

void SolenoidSubsystem::SetPosition(frc::DoubleSolenoid::Value position) {
	piston.Set(position);
}

frc::DoubleSolenoid::Value SolenoidSubsystem::GetPosition() {
	return piston.Get();
}