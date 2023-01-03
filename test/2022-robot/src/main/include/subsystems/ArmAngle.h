#pragma once

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>
#include <frc/motorcontrol/MotorControllerGroup.h>

class ArmAngle : public frc2::SubsystemBase {
public:
    ArmAngle();

    void SetArm(double speed);

private:
    WPI_TalonSRX angleMotor1;
	WPI_TalonSRX angleMotor2;
	frc::MotorControllerGroup angleMotors;
};