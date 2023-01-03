#pragma once

#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>

class Drivetrain : public frc2::SubsystemBase {
public:
    Drivetrain();

private:
    WPI_TalonFX frontRight;
	WPI_TalonFX frontLeft;
    WPI_TalonFX backRight;
    WPI_TalonFX backLeft;
};