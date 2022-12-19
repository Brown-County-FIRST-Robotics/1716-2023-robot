#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>

class Solenoid : public frc2::SubsystemBase {
public:
    Solenoid();

    void SetPosition(frc::DoubleSolenoid::Value position);
	frc::DoubleSolenoid::Value GetPosition();

private:
 	frc::DoubleSolenoid piston;
};