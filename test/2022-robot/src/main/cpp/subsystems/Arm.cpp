#include "Constants.h"
#include "subsystems/Arm.h"

Arm::Arm() : 
	shooterMotor1{ArmConstants::SHOOTERMOTOR1ID}, 
	shooterMotor2{ArmConstants::SHOOTERMOTOR2ID}, 
	shooterMotors{shooterMotor1, shooterMotor2}, 
	shooterSolenoid{frc::PneumaticsModuleType::CTREPCM, ArmConstants::SHOOTERSOLENOIDFORWARDID, ArmConstants::SHOOTERSOLENOIDREVERSEID}, 
	climberSolenoid{frc::PneumaticsModuleType::CTREPCM, ArmConstants::CLIMBERSOLENOIDFORWARDID, ArmConstants::CLIMBERSOLENOIDREVERSEID} {}

void Arm::SetShooter(double speed) {
	shooterMotors.Set(speed);
}

void Arm::SetShooterSolenoid(frc::DoubleSolenoid::Value position) {
	shooterSolenoid.Set(position);
}

frc::DoubleSolenoid::Value Arm::GetShooterSolenoid() {
	return shooterSolenoid.Get();
}

void Arm::SetClimberSolenoid(frc::DoubleSolenoid::Value position) {
	climberSolenoid.Set(position);
}

frc::DoubleSolenoid::Value Arm::GetClimberSolenoid() {
	return climberSolenoid.Get();
}