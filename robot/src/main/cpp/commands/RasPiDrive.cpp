#include <span>
#include <chrono>

#include "commands/RasPiDrive.h"
#include "Constants.h"

RasPiDrive::RasPiDrive(Drivetrain* drive, bool isAutonomous) : drivetrain(drive) {
	AddRequirements(drive);

	networkTableInst = nt::NetworkTableInstance::GetDefault();
	driveTable = networkTableInst.GetTable("1716Drive");

	x = driveTable->GetFloatTopic("x").Subscribe(0.0, {.pollStorage = 1}); //set the x, y, and rotation listeners 
	y = driveTable->GetFloatTopic("y").Subscribe(0.0, {.pollStorage = 1}); //with a default of 0 and a memory of 1 term
	z = driveTable->GetFloatTopic("rotation").Subscribe(0.0, {.pollStorage = 1});
	isTank = driveTable->GetBooleanTopic("isTank").Subscribe(false);

	autonomous = isAutonomous; //for odometry
	if (autonomous) {
		motorStartPos = drivetrain->GetEncoder(DrivetrainConst::FRONT_RIGHT_ID);
		rotationStartPos = drivetrain->GetYaw();
	}
}

void RasPiDrive::Execute() {
	drivetrain->Drive(x.Get(), y.Get(), z.Get());

	if (isTank.Get() && drivetrain->GetSolenoid() == frc::DoubleSolenoid::Value::kReverse) {
		drivetrain->SetSolenoid(frc::DoubleSolenoid::Value::kForward);
	}
	else if (!isTank.Get() && drivetrain->GetSolenoid() == frc::DoubleSolenoid::Value::kForward) {
		drivetrain->SetSolenoid(frc::DoubleSolenoid::Value::kReverse);
	}

	if (autonomous && fabs(drivetrain->GetYaw() - rotationStartPos) > AutonomousConst::ROTATION_DISTANCE) { //for odometry
		hasStopped = true;
	}
}

void RasPiDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}

bool RasPiDrive::IsFinished() {
	if (autonomous) { //for odometry
		return !hasStopped && fabs(drivetrain->GetEncoder(DrivetrainConst::FRONT_RIGHT_ID) - motorStartPos) > AutonomousConst::TIMEOUT_DISTANCE;
	}
	return false;
}