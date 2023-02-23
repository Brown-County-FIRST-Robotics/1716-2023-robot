#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/button/CommandXboxController.h>

#include "subsystems/Drivetrain.h"
// #include "subsystems/Arm.h"
// #include "subsystems/ArmAngle.h"


class RobotContainer {
public:
	RobotContainer();

private:
	frc2::CommandXboxController controller{0};
	
	Drivetrain drivetrain;
	// Arm arm;
	// ArmAngle armAngle;

	void ConfigureButtonBindings();
};