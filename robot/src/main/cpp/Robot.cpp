#include "Robot.h"

#include <frc2/command/CommandScheduler.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/smartdashboard/SmartDashboard.h>

void Robot::RobotInit() {
	//Pickup and placement position selector
	pickUpPos[0] = pickUpGrid
		.Add("Drop", false)
		.WithPosition(3, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	pickUpPos[1] = pickUpGrid
		.Add("Left Slide", false)
		.WithPosition(4, 0)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	pickUpPos[2] = pickUpGrid
		.Add("Right Slide", false)
		.WithPosition(5, 0)
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
	
	//LED lights:
	ledChooser.SetDefaultOption("Knight Rider", 0);
	ledChooser.AddOption("Wierdness", 1);
	frc::SmartDashboard::PutData("LED Mode", &ledChooser);
	frc::Shuffleboard::StartRecording();
}

void Robot::RobotPeriodic() {
	frc2::CommandScheduler::GetInstance().Run();
}

void Robot::AutonomousInit() {
	//Autonomous
	autonomousCommand = robotContainer.GetAutonomousCommand();

	if (autonomousCommand != nullptr) {
		autonomousCommand->Schedule();
	}
	robotContainer.Init();
}

void Robot::TeleopInit() {
	//Autonomous
	if (autonomousCommand != nullptr) {
		autonomousCommand->Cancel();
    	autonomousCommand = nullptr;
	}

	robotContainer.Init();
}

void Robot::TeleopPeriodic() { }

void Robot::DisabledInit() {
	//Controller Logging
	frc::Shuffleboard::StopRecording();
}

#ifndef RUNNING_FRC_TESTS
int main() {
	return frc::StartRobot<Robot>();
}
#endif
