#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>
#include <frc/PneumaticHub.h>

class SolenoidSubsystem : public frc2::SubsystemBase {
public:
		SolenoidSubsystem();

		void SetPosition(frc::DoubleSolenoid::Value position);
	frc::DoubleSolenoid::Value GetPosition();

private:
 	
	frc::PneumaticHub hub{42};
	frc::DoubleSolenoid piston = hub.MakeDoubleSolenoid(0,1);
};
