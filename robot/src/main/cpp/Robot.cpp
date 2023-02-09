#include "Robot.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/CommandScheduler.h>
#include <frc/shuffleboard/Shuffleboard.h>

void Robot::RobotInit() {
	robotRunning = frc::Shuffleboard::GetTab("Controller").Add("Robot Running", false).GetEntry();
}

void Robot::RobotPeriodic() {
	frc2::CommandScheduler::GetInstance().Run();
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
}

void Robot::TeleopPeriodic() {
	robotContainer.UpdateControllerLogging();
}

void Robot::DisabledInit() {
	robotRunning->SetBoolean(false);
}

#ifndef RUNNING_FRC_TESTS
int main() {
	return frc::StartRobot<Robot>();
}
#endif