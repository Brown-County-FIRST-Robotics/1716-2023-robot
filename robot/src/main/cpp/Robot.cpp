#include "Robot.h"

#include <frc2/command/CommandScheduler.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/smartdashboard/SmartDashboard.h>

void Robot::RobotInit() {	
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
