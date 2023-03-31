#include "commands/ArmTeleopControl.h"

#include <utility>
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>

ArmTeleopControl::ArmTeleopControl(Arm* subsystem, 
	std::function<int()> elbowPov,
	std::function<bool()> clawBool,
	std::function<bool()> floorBool,
	std::function<bool()> mediumBool,
	std::function<bool()> highBool,
	std::function<bool()> driveBool,
	std::function<bool()> portalBool)
	: arm(subsystem), elbow(std::move(elbowPov)), claw(std::move(clawBool)), 
	floorButton(std::move(floorBool)), mediumButton(std::move(mediumBool)), highButton(std::move(highBool)), 
	driveButton(std::move(driveBool)), portalButton(std::move(portalBool))
{
	AddRequirements(subsystem);

	//presets
//TEMP CODE: SHUFFLEBOARD PRESETS
	// high = frc::Shuffleboard::GetTab("Arm Presets")
	// 	.Add("High", false)
	// 	.WithWidget(frc::BuiltInWidgets::kToggleButton)
	// 	.GetEntry();
	// medium = frc::Shuffleboard::GetTab("Arm Presets")
	// 	.Add("Medium", false)
	// 	.WithWidget(frc::BuiltInWidgets::kToggleButton)
	// 	.GetEntry();
	// floor = frc::Shuffleboard::GetTab("Arm Presets")
	// 	.Add("Floor", false)
	// 	.WithWidget(frc::BuiltInWidgets::kToggleButton)
	// 	.GetEntry();
	// portal = frc::Shuffleboard::GetTab("Arm Presets")
	// 	.Add("Portal", false)
	// 	.WithWidget(frc::BuiltInWidgets::kToggleButton)
	// 	.GetEntry();
	// drive = frc::Shuffleboard::GetTab("Arm Presets")
	// 	.Add("Drive", false)
	// 	.WithWidget(frc::BuiltInWidgets::kToggleButton)
	// 	.GetEntry();
//END TEMP CODE: SHUFFLEBOARD PRESETS
}

void ArmTeleopControl::Execute() {
	frc::SmartDashboard::PutNumber("POV", elbow());
	std::cout << "POV: " << elbow() << "\n";


	if (!clawPressed && claw()) {
		arm->ToggleClaw();
		clawPressed = true;
	}
	else if (!claw())
		clawPressed = false;


//TEMP CODE: MANUAL SHOULDER CONTROL
	if (elbow() == 135 || elbow() == 90 || elbow() == 45){
		arm->AddToShoulderGoal(ArmConst::SHOULDER_MANUAL_SPEED);
		shoulderStopped = false;
	}
	else if (elbow() == 225 || elbow() == 270 || elbow() == 315) {
		arm->AddToShoulderGoal(-ArmConst::SHOULDER_MANUAL_SPEED);
		shoulderStopped = false;
	}
	else if(!shoulderStopped){
		// when you let go of DPad, stop the arm, but only the first time. 
		//After the first time, stop applying updates, or the arm can lock in new positions while drifting
		arm->StopShoulder();
		shoulderStopped=true;
	}
//END TEMP CODE: MANUAL SHOULDER CONTROL


	if (elbow() == 315 || elbow() == 0 || elbow() == 45){
		arm->AddToElbowGoal(ArmConst::ELBOW_MANUAL_SPEED);
		elbowStopped = false;
	}
	else if (elbow() == 135 || elbow() == 180 || elbow() == 225) {
		arm->AddToElbowGoal(-ArmConst::ELBOW_MANUAL_SPEED);
		elbowStopped = false;
	}
	else if(!elbowStopped){
		// when you let go of DPad, stop the arm, but only the first time. 
		//After the first time, stop applying updates, or the arm can lock in new positions while drifting
		arm->StopElbow();
		elbowStopped=true;
	}

	//Presets
//TEMP CODE: SHUFFLEBOARD PRESETS
	// if (high->GetBoolean(false)) {
	// 	arm->SetShoulderGoal(ArmHeightConst::HIGH[0]);
	// 	arm->SetElbowGoal(ArmHeightConst::HIGH[1]);
	// 	high->SetBoolean(false);
	// }
	// else if (medium->GetBoolean(false)) {
	// 	arm->SetShoulderGoal(ArmHeightConst::MEDIUM[0]);
	// 	arm->SetElbowGoal(ArmHeightConst::MEDIUM[1]);
	// 	medium->SetBoolean(false);
	// }
	// else if (floor->GetBoolean(false)) {
	// 	arm->SetShoulderGoal(ArmHeightConst::FLOOR[0]);
	// 	arm->SetElbowGoal(ArmHeightConst::FLOOR[1]);
	// 	floor->SetBoolean(false);
	// }
	// else if (portal->GetBoolean(false)) {
	// 	arm->SetShoulderGoal(ArmHeightConst::PORTAL[0]);
	// 	arm->SetElbowGoal(ArmHeightConst::PORTAL[1]);
	// 	portal->SetBoolean(false);
	// }
	// else if (drive->GetBoolean(false)) {
	// 	arm->SetShoulderGoal(ArmHeightConst::DRIVE[0]);
	// 	arm->SetElbowGoal(ArmHeightConst::DRIVE[1]);
	// 	drive->SetBoolean(false);
	// }
//TEMP CODE: SHUFFLEBOARD PRESETS
	if (floorButton() && !floorButtonPrevState) {
		arm->SetShoulderGoal(ArmHeightConst::FLOOR[0]);
		arm->SetElbowGoal(ArmHeightConst::FLOOR[1]);
		floorButtonPrevState = true;
		elbowStopped=true;
	}
	else if (!floorButton() && floorButtonPrevState) {
		floorButtonPrevState = false;
	}
	if (mediumButton() && !mediumButtonPrevState) {
		arm->SetShoulderGoal(ArmHeightConst::MEDIUM[0]);
		arm->SetElbowGoal(ArmHeightConst::MEDIUM[1]);
		mediumButtonPrevState = true;
		elbowStopped = true;
	}
	else if (!mediumButton() && mediumButtonPrevState) {
		mediumButtonPrevState = false;
	}
	if (highButton() && !highButtonPrevState) {
		arm->SetShoulderGoal(ArmHeightConst::HIGH[0]);
		arm->SetElbowGoal(ArmHeightConst::HIGH[1]);
		highButtonPrevState = true;
		elbowStopped = true;
	}
	else if (!highButton() && highButtonPrevState) {
		highButtonPrevState = false;
	}
	if (driveButton() && !driveButtonPrevState) {
		arm->SetShoulderGoal(ArmHeightConst::DRIVE[0]);
		arm->SetElbowGoal(ArmHeightConst::DRIVE[1]);
		driveButtonPrevState = true;
		elbowStopped = true;
	}
	else if (!driveButton() && driveButtonPrevState) {
		driveButtonPrevState = false;
	}
	if (portalButton() && !portalButtonPrevState) {
		arm->SetShoulderGoal(ArmHeightConst::PORTAL[0]);
		arm->SetElbowGoal(ArmHeightConst::PORTAL[1]);
		portalButtonPrevState = true;
		elbowStopped = true;
	}
	else if (!portalButton() && portalButtonPrevState) {
		portalButtonPrevState = false;
	}
}

void ArmTeleopControl::End(bool interrupted) {
	arm->StopShoulder();
	arm->StopElbow();
}
