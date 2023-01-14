#include "subsystems/SolenoidSubsystem.h"

SolenoidSubsystem::SolenoidSubsystem() {
	hub.EnableCompressorDigital();
}

void SolenoidSubsystem::SetPosition(frc::DoubleSolenoid::Value position) {
	piston.Set(position);
}

frc::DoubleSolenoid::Value SolenoidSubsystem::GetPosition() {
	return piston.Get();
}