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
	nt::NetworkTableInstance networkTableInst;
	std::shared_ptr<nt::NetworkTable> gameInfoTable;

	nt::BooleanPublisher isAutonomous;
	nt::BooleanPublisher isTeleop;
	nt::BooleanPublisher isRedAlliance;
	nt::DoublePublisher matchTime;

	//Selector for pickup and placement positions
	frc::ShuffleboardLayout& pickUpGrid = frc::Shuffleboard::GetTab("Teleop")
		.GetLayout("Pick Up Positions", frc::BuiltInLayouts::kGrid)
		.WithSize(4, 2)
		.WithProperties({
			{"Number of rows", nt::Value::MakeInteger(1)},
			{"Number of columns", nt::Value::MakeInteger(3)},
			{"Label Position", nt::Value::MakeString("HIDDEN")}});
	frc::ShuffleboardLayout& placeGrid = frc::Shuffleboard::GetTab("Teleop")
		.GetLayout("Placement Positions", frc::BuiltInLayouts::kGrid)
		.WithSize(10, 4)
		.WithProperties({
			{"Number of rows", nt::Value::MakeInteger(3)},
			{"Number of columns", nt::Value::MakeInteger(9)},
			{"Label Position", nt::Value::MakeString("HIDDEN")}});

	nt::GenericEntry* pickUpPos[3];
	nt::GenericEntry* placePos[3][9];

	std::shared_ptr<nt::NetworkTable> dashboardTable;
	nt::IntegerPublisher pickUpPublisher;
	nt::IntegerArrayPublisher placePublisher;
	std::vector<int64_t> placeCoords;

	int currentPickUp = -1;
	int currentPlace[2] = {-1, -1};

	//LED Lights:
	frc::AddressableLED led{LEDConst::ID};
	std::array<frc::AddressableLED::LEDData, LEDConst::LENGTH> ledBuffer;
	int ledUpdateSpeedCounter = 0;

	void SetAllLeds(int h, int s, int v);
	void SetLed(int id, int h, int s, int v);
	
	void KnightRider();
	bool knightRiderLedGoingOut = true;
	int knightRiderIndex = 0;

	void Weirdness();
	bool weirdnessValueIsIncreasing = false;
	int weirdnessBrightness;
	int r;
	bool rUp = true;
	int g;
	bool gUp = true;
	int b;
	bool bUp = true;

	frc::SendableChooser<int> ledChooser;
};
