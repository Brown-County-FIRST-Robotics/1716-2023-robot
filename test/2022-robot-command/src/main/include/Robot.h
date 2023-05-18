#pragma once

#include <frc/TimedRobot.h>
#include <frc2/command/Command.h>

//#include "RobotContainer.h"

class Robot : public frc::TimedRobot {
 public:
	void RobotPeriodic() override;
	// void AutonomousInit() override;
	// void TeleopInit() override;
	
private:
	//RobotContainer robotContainer;
	//frc2::Command* autonomousCommand = nullptr;
};
