#include "Robot.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/CommandScheduler.h>
#include <frc/shuffleboard/Shuffleboard.h>

void Robot::RobotInit() {
	//Pickup and placement position selector
	pickUpPos[0] = pickUpGrid
		.Add("Drop", false)
		.WithPosition(0, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	pickUpPos[1] = pickUpGrid
		.Add("Left Slide", false)
		.WithPosition(1, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	pickUpPos[2] = pickUpGrid
		.Add("Right Slide", false)
		.WithPosition(2, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	
	for (int r = 0; r < 2; r++) {
		for (int c = 0; c < 9; c++) {
			if (c % 3 != 1) {
				placePos[r][c] = placeGrid
					.Add("<" + std::to_string(c + (r * 9) + 1) + ">", false)
					.WithPosition(c, r)
					.WithWidget(frc::BuiltInWidgets::kToggleButton)
					.GetEntry();
			}
			else {
				placePos[r][c] = placeGrid
					.Add("[" + std::to_string(c + (r * 9) + 1) + "]", false)
					.WithPosition(c, r)
					.WithWidget(frc::BuiltInWidgets::kToggleButton)
					.GetEntry();
			}
		}
	}
	for (int c = 0; c < 9; c++) {
		placePos[2][c] = placeGrid
			.Add("(" + std::to_string(c + 19) + ")", false)
			.WithPosition(c, 2)
			.WithWidget(frc::BuiltInWidgets::kToggleButton)
			.GetEntry();
	}
	
	//Update networktables info
	networkTableInst = nt::NetworkTableInstance::GetDefault();
	dashboardTable = networkTableInst.GetTable("1716DashboardInput");
	gameInfoTable = networkTableInst.GetTable("1716GameInfo");

	isAutonomous = gameInfoTable->GetBooleanTopic("isAutonomous").Publish();
	isTeleop = gameInfoTable->GetBooleanTopic("isTeleop").Publish();
	isRedAlliance = gameInfoTable->GetBooleanTopic("isRedAlliance").Publish();
	matchTime = gameInfoTable->GetDoubleTopic("matchTime").Publish();

	isRedAlliance.Set(frc::DriverStation::GetAlliance() == frc::DriverStation::Alliance::kRed);
	isAutonomous.Set(false);
	isTeleop.Set(false);
	matchTime.Set(0);

	//Pickup and place selector
	pickUpPublisher = dashboardTable->GetIntegerTopic("pickUpPos").Publish();
	placePublisher = dashboardTable->GetIntegerArrayTopic("placePos").Publish();
}

void Robot::RobotPeriodic() {
	frc2::CommandScheduler::GetInstance().Run();

	//Update matchtime networktables variable
	matchTime.Set(frc::DriverStation::GetMatchTime());

	//Pickup and place position selectors:
	//Pick up
	for (int i = 0; i < 3; i++) {
		if (pickUpPos[i]->GetBoolean(false) && i != currentPickUp) {
			if (currentPickUp != -1) {
				pickUpPos[currentPickUp]->SetBoolean(false);
			}
			currentPickUp = i;
			pickUpPublisher.Set(i + 1);
		}
	}

	//Place
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 9; c++) {
			if (placePos[r][c]->GetBoolean(false) && (currentPlace[0] != r || currentPlace[1] != c)) {
				if (currentPlace[r] != -1) {
					placePos[currentPlace[0]][currentPlace[1]]->SetBoolean(false);
				}
				currentPlace[0] = r;
				currentPlace[1] = c;

				placeCoords.clear();
				placeCoords.push_back(r + 1);
				placeCoords.push_back(c + 1);
				placePublisher.Set(placeCoords); //this builds, DO NOT CHANGE
			}
		}
	}
}

void Robot::AutonomousInit() {
	//Autonomous
	autonomousCommand = robotContainer.GetAutonomousCommand();

	if (autonomousCommand != nullptr) {
		autonomousCommand->Schedule();
	}

	//Networktables variable update
	isAutonomous.Set(true);
}

void Robot::TeleopInit() {
	//Autonomous
	if (autonomousCommand != nullptr) {
		autonomousCommand->Cancel();
    	autonomousCommand = nullptr;
	}

	//Networktables
	isAutonomous.Set(false);
	isTeleop.Set(true);

	//Controller logging
	frc::Shuffleboard::StartRecording();
}

void Robot::TeleopPeriodic() {
	//Controller logging
	robotContainer.UpdateControllerLogging();
}

void Robot::DisabledInit() {
	//Networktables
	isTeleop.Set(false);
	
	//Controller Logging
	frc::Shuffleboard::StopRecording();
}

#ifndef RUNNING_FRC_TESTS
int main() {
	return frc::StartRobot<Robot>();
}
#endif