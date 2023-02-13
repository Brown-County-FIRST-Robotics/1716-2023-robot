#include "Robot.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/CommandScheduler.h>

void Robot::RobotInit() {
	networkTableInst = nt::NetworkTableInstance::GetDefault();
	table = networkTableInst.GetTable("1716GameInfo");

	isAutonomous = table->GetBooleanTopic("isAutonomous").Publish();
	isTeleop = table->GetBooleanTopic("isTeleop").Publish();
	isRedAlliance = table->GetBooleanTopic("isRedAlliance").Publish();
	matchTime = table->GetDoubleTopic("matchTime").Publish();

	isRedAlliance.Set(driverStation.GetAlliance() == frc::DriverStation::Alliance::kRed);
	isAutonomous.Set(false);
	isTeleop.Set(false);
	matchTime.Set(0);
}

void Robot::RobotPeriodic() {
	frc2::CommandScheduler::GetInstance().Run();

	matchTime.Set(driverStation.GetMatchTime());
}

void Robot::AutonomousInit() {
	autonomousCommand = robotContainer.GetAutonomousCommand();

	if (autonomousCommand != nullptr) {
		autonomousCommand->Schedule();
	}

	isAutonomous.Set(true);
}

void Robot::TeleopInit() {
	if (autonomousCommand != nullptr) {
		autonomousCommand->Cancel();
    	autonomousCommand = nullptr;
	}

	isAutonomous.Set(false);
	isTeleop.Set(true);
}

void Robot::DisabledInit() {
	isTeleop.Set(false);
}

#ifndef RUNNING_FRC_TESTS
int main() {
	return frc::StartRobot<Robot>();
}
#endif