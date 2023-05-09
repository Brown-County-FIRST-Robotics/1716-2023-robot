#pragma once

#include <frc/TimedRobot.h>
#include <frc2/command/Command.h>
#include <frc/DriverStation.h>
#include <networktables/BooleanTopic.h>
#include <networktables/DoubleTopic.h>
#include <frc/shuffleboard/ShuffleboardLayout.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <networktables/IntegerArrayTopic.h>
#include <networktables/IntegerTopic.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <frc/AddressableLED.h>
#include <array>

#include "RobotContainer.h"

class Robot : public frc::TimedRobot {
 public:
	void RobotInit() override;
	void RobotPeriodic() override;
	void AutonomousInit() override;
	void TeleopInit() override;
	void TeleopPeriodic() override;
	void DisabledInit() override;
	
private:
	RobotContainer robotContainer;

	//Autonomous
	frc2::Command* autonomousCommand = nullptr;

	//Update networktable info

	//Selector for pickup and placement positions
	frc::SendableChooser<int> ledChooser;
};
