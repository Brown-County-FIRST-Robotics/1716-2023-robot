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
	nt::NetworkTableInstance networkTableInst;
	std::shared_ptr<nt::NetworkTable> table;

	nt::BooleanPublisher isAutonomous;
	nt::BooleanPublisher isTeleop;
	nt::BooleanPublisher isRedAlliance;
	nt::DoublePublisher matchTime;

	//Selector for pickup and placement positions
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
	std::vector<int64_t> placeCoords;

	int currentPickUp = -1;
	int currentPlace[2] = {-1, -1};
};
