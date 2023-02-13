#pragma once

#include <frc/TimedRobot.h>
#include <frc2/command/Command.h>
#include <frc/DriverStation.h>
#include <networktables/BooleanTopic.h>
#include <networktables/DoubleTopic.h>

#include "RobotContainer.h"

class Robot : public frc::TimedRobot {
public:
 	void RobotInit() override;
	void RobotPeriodic() override;
	void AutonomousInit() override;
	void TeleopInit() override;
	void DisabledInit() override;
	
private:
	RobotContainer robotContainer;
	frc2::Command* autonomousCommand = nullptr;

	frc::DriverStation driverStation;

	nt::NetworkTableInstance networkTableInst;
	std::shared_ptr<nt::NetworkTable> table;

	nt::BooleanPublisher isAutonomous;
	nt::BooleanPublisher isTeleop;
	nt::BooleanPublisher isRedAlliance;
	nt::DoublePublisher matchTime;
};
