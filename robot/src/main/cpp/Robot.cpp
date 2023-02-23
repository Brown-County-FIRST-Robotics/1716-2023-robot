#include "Robot.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/CommandScheduler.h>
#include <frc/shuffleboard/Shuffleboard.h>

void Robot::RobotInit() {
	robotRunning = frc::Shuffleboard::GetTab("Controller").Add("Robot Running", false).GetEntry();

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

	networkTableInst = nt::NetworkTableInstance::GetDefault();
	placementTable = networkTableInst.GetTable("1716Placement"); //placeholder table
	
	pickUpPublisher = placementTable->GetIntegerTopic("pickUpPos").Publish();
	placePublisher = placementTable->GetIntegerArrayTopic("placePos").Publish();
}

void Robot::RobotPeriodic() {
	frc2::CommandScheduler::GetInstance().Run();

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
	autonomousCommand = robotContainer.GetAutonomousCommand();

	if (autonomousCommand != nullptr) {
		autonomousCommand->Schedule();
	}

	robotRunning->SetBoolean(true);
}

void Robot::TeleopInit() {
	if (autonomousCommand != nullptr) {
		autonomousCommand->Cancel();
    	autonomousCommand = nullptr;
	}

	robotRunning->SetBoolean(true);

	frc::Shuffleboard::StartRecording();
}

void Robot::TeleopPeriodic() {
	robotContainer.UpdateControllerLogging();
}

void Robot::DisabledInit() {
	robotRunning->SetBoolean(false);
	frc::Shuffleboard::StopRecording();
}

#ifndef RUNNING_FRC_TESTS
int main() {
	return frc::StartRobot<Robot>();
}
#endif