#include <span>
#include <chrono>

#include "commands/RasPiDrive.h"
#include "Constants.h"

RasPiDrive::RasPiDrive(Drivetrain* drive) : drivetrain(drive) {
	AddRequirements(drive);

	networkTableInst = nt::NetworkTableInstance::GetDefault();
	driveTable = networkTableInst.GetTable("1716Drive");

	x = driveTable->GetFloatTopic("x").Subscribe(0.0, {.pollStorage = 1}); //set the x, y, and rotation listeners 
	y = driveTable->GetFloatTopic("y").Subscribe(0.0, {.pollStorage = 1}); //with a default of 0 and a memory of 1 term
	z = driveTable->GetFloatTopic("rotation").Subscribe(0.0, {.pollStorage = 1});
	isTank = driveTable->GetBooleanTopic("isTank").Subscribe(false);

	armFloor = driveTable->GetBooleanTopic("setArmFloor").GetEntry(false);
	armPortal = driveTable->GetBooleanTopic("setArmPortal").GetEntry(false);
	armMedium = driveTable->GetBooleanTopic("setArm").GetEntry(false);
	armHigh = driveTable->GetBooleanTopic("setArm").GetEntry(false);
	armLowNode = driveTable->GetBooleanTopic("setArm").GetEntry(false);
	armHighNode = driveTable->GetBooleanTopic("setArm").GetEntry(false);
}

void RasPiDrive::Execute() {
	drivetrain->Drive(x.Get(), y.Get(), z.Get());

	if (isTank.Get() && drivetrain->GetSolenoid() == frc::DoubleSolenoid::Value::kReverse) {
		drivetrain->SetSolenoid(frc::DoubleSolenoid::Value::kForward);
	}
	else if (!isTank.Get() && drivetrain->GetSolenoid() == frc::DoubleSolenoid::Value::kForward) {
		drivetrain->SetSolenoid(frc::DoubleSolenoid::Value::kReverse);
	}
}

void RasPiDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}