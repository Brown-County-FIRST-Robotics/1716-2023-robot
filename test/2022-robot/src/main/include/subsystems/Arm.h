#pragma once

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <frc/motorcontrol/MotorControllerGroup.h>
#include <frc/DoubleSolenoid.h>


class Arm : public frc2::SubsystemBase {
public:
    Arm();

    void SetShooter(double speed);

	void SetShooterSolenoid(frc::DoubleSolenoid::Value position);
	frc::DoubleSolenoid::Value GetShooterSolenoid();

	void SetClimberSolenoid(frc::DoubleSolenoid::Value position);
	frc::DoubleSolenoid::Value GetClimberSolenoid();

private:
    WPI_TalonFX shooterMotor1;
    WPI_TalonFX shooterMotor2;
	frc::MotorControllerGroup shooterMotors;

	frc::DoubleSolenoid shooterSolenoid;

	frc::DoubleSolenoid climberSolenoid; //birb
};