#pragma once

#include <frc/TimedRobot.h>
#include <frc2/command/Command.h>
#include <frc/shuffleboard/ShuffleboardLayout.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <networktables/IntegerArrayTopic.h>
#include <networktables/IntegerTopic.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

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
	frc2::Command* autonomousCommand = nullptr;

	nt::GenericEntry* robotRunning;

	frc::ShuffleboardLayout& pickUpGrid = frc::Shuffleboard::GetTab("Pick Up")
		.GetLayout("Pick Up Positions", frc::BuiltInLayouts::kGrid)
		.WithSize(4, 2)
		.WithProperties({
			{"Number of rows", nt::Value::MakeInteger(1)},
			{"Number of columns", nt::Value::MakeInteger(3)},
			{"Label Position", nt::Value::MakeString("HIDDEN")}});
	frc::ShuffleboardLayout& placeGrid = frc::Shuffleboard::GetTab("Place")
		.GetLayout("Placement Positions", frc::BuiltInLayouts::kGrid)
		.WithSize(10, 4)
		.WithProperties({
			{"Number of rows", nt::Value::MakeInteger(3)},
			{"Number of columns", nt::Value::MakeInteger(9)},
			{"Label Position", nt::Value::MakeString("HIDDEN")}});

	nt::GenericEntry* pickUpPos[3];
	nt::GenericEntry* placePos[3][9];

	nt::NetworkTableInstance networkTableInst;
	std::shared_ptr<nt::NetworkTable> placementTable; //placeholder name
	nt::IntegerPublisher pickUpPublisher;
	nt::IntegerArrayPublisher placePublisher;

	int currentPickUp = -1;
	int currentPlace[2] = {-1, -1};
};
