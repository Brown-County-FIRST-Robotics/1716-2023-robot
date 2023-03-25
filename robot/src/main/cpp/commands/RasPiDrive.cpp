#include <span>
#include <chrono>

#include "commands/RasPiDrive.h"
#include "Constants.h"

RasPiDrive::RasPiDrive(Drivetrain* drive, Arm* arm) : drivetrain(drive), arm(arm) {
	AddRequirements(drive);

	networkTableInst = nt::NetworkTableInstance::GetDefault();
	driveTable = networkTableInst.GetTable("1716Drive");

	x = driveTable->GetFloatTopic("x").Subscribe(0.0, {.pollStorage = 1}); //set the x, y, and rotation listeners 
	y = driveTable->GetFloatTopic("y").Subscribe(0.0, {.pollStorage = 1}); //with a default of 0 and a memory of 1 term
	z = driveTable->GetFloatTopic("rotation").Subscribe(0.0, {.pollStorage = 1});
	isTank = driveTable->GetBooleanTopic("isTank").Subscribe(false);

	armFloor = driveTable->GetBooleanTopic("setArmFloor").GetEntry(false);
	armPortal = driveTable->GetBooleanTopic("setArmPortal").GetEntry(false);
	armMedium = driveTable->GetBooleanTopic("setArmMedium").GetEntry(false);
	armHigh = driveTable->GetBooleanTopic("setArmHigh").GetEntry(false);
	armLowNode = driveTable->GetBooleanTopic("setArmLowNode").GetEntry(false);
	armHighNode = driveTable->GetBooleanTopic("setArmHighNode").GetEntry(false);

	dropObject = driveTable->GetBooleanTopic("dropObject").GetEntry(false);
	pickupObject = driveTable->GetBooleanTopic("pickupObject").GetEntry(false);
}

void RasPiDrive::Execute() {
	drivetrain->Drive(x.Get(), y.Get(), z.Get());

	if (isTank.Get() && drivetrain->GetSolenoid() == frc::DoubleSolenoid::Value::kReverse) {
		drivetrain->SetSolenoid(frc::DoubleSolenoid::Value::kForward);
	}
	else if (!isTank.Get() && drivetrain->GetSolenoid() == frc::DoubleSolenoid::Value::kForward) {
		drivetrain->SetSolenoid(frc::DoubleSolenoid::Value::kReverse);
	}

	if (armFloor.Get(false)) {
		arm->SetElbowGoal(ArmHeightConst::FLOOR);
	} else if (armPortal.Get(false)) {
		arm->SetElbowGoal(ArmHeightConst::PORTAL);
	} else if (armMedium.Get(false)) {
		arm->SetElbowGoal(ArmHeightConst::MEDIUM);
	} else if (armHigh.Get(false)) {
		arm->SetElbowGoal(ArmHeightConst::HIGH);
	} else if (armLowNode.Get(false)) {
		arm->SetElbowGoal(ArmHeightConst::LOWNODE);
	} else if (armHighNode.Get(false)) {
		arm->SetElbowGoal(ArmHeightConst::HIGHNODE);
	}

	if (ArmHeightConst::THRESHOLD > abs(arm->GetElbowGoal() - arm->GetElbowPosition())) {
		arm->SetElbowActive(false);
		armFloor.Set(false);
		armPortal.Set(false);
		armMedium.Set(false);
		armHigh.Set(false);
		armLowNode.Set(false);
		armHighNode.Set(false);
	}
	if (dropObject.Get(false)){
		arm->SetClaw(ArmConst::CLAW_OPEN);
		dropObject.Set(false);
	}
	if (pickupObject.Get(false)) {
		arm->SetClaw(ArmConst::CLAW_CLOSED);
		pickupObject.Set(false);
	}
}

void RasPiDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
	arm->SetElbowActive(false);
}