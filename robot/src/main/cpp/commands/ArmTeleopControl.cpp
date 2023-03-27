#include "commands/ArmTeleopControl.h"

#include <utility>
#include <iostream>

ArmTeleopControl::ArmTeleopControl(Arm* subsystem, std::function<double()> shoulderAxis, 
	std::function<double()> elbowAxis, std::function<bool()> clawButton) : arm(subsystem),
	shoulder(std::move(shoulderAxis)), elbow(std::move(elbowAxis)), claw(std::move(clawButton))
{
	AddRequirements(subsystem);

	//presets
	high = frc::Shuffleboard::GetTab("Arm Presets")
		.Add("High", false)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	medium = frc::Shuffleboard::GetTab("Arm Presets")
		.Add("Medium", false)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	floor = frc::Shuffleboard::GetTab("Arm Presets")
		.Add("Floor", false)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	portal = frc::Shuffleboard::GetTab("Arm Presets")
		.Add("Portal", false)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
	drive = frc::Shuffleboard::GetTab("Arm Presets")
		.Add("Drive", false)
		.WithWidget(frc::BuiltInWidgets::kToggleButton)
		.GetEntry();
}

void ArmTeleopControl::Execute() {
	if (!clawPressed && claw()) {
		arm->ToggleClaw();
		clawPressed = true;
	}
	else if (!claw())
		clawPressed = false;


	if (fabs(shoulder()) > .15){
		arm->AddToShoulderGoal(-shoulder() * ArmConst::SHOULDER_JOYSTICK_SPEED);
		shoulderStopped=false;
	}else if (!shoulderStopped){ 
		// when you let go of joystick, stop the arm, but only the first time. 
		//After the first time, stop applying updates, or the arm can lock in new positions while drifting
		arm->StopShoulder();
		shoulderStopped=true;
	}
	if (fabs(elbow()) > .05){
		arm->AddToElbowGoal(-elbow() * ArmConst::ELBOW_JOYSTICK_SPEED);
		elbowStopped=false;
	}else if(!elbowStopped){
		// when you let go of joystick, stop the arm, but only the first time. 
		//After the first time, stop applying updates, or the arm can lock in new positions while drifting
		arm->StopElbow();
		elbowStopped=true;
	}

	//Presets
	if (high->GetBoolean(false)) {
		arm->SetShoulderGoal(ArmHeightConst::HIGH[0]);
		arm->SetElbowGoal(ArmHeightConst::HIGH[1]);
		high->SetBoolean(false);
	}
	else if (medium->GetBoolean(false)) {
		arm->SetShoulderGoal(ArmHeightConst::MEDIUM[0]);
		arm->SetElbowGoal(ArmHeightConst::MEDIUM[1]);
		medium->SetBoolean(false);
	}
	else if (floor->GetBoolean(false)) {
		arm->SetShoulderGoal(ArmHeightConst::FLOOR[0]);
		arm->SetElbowGoal(ArmHeightConst::FLOOR[1]);
		floor->SetBoolean(false);
	}
	else if (portal->GetBoolean(false)) {
		arm->SetShoulderGoal(ArmHeightConst::PORTAL[0]);
		arm->SetElbowGoal(ArmHeightConst::PORTAL[1]);
		portal->SetBoolean(false);
	}
	else if (drive->GetBoolean(false)) {
		arm->SetShoulderGoal(ArmHeightConst::DRIVE[0]);
		arm->SetElbowGoal(ArmHeightConst::DRIVE[1]);
		drive->SetBoolean(false);
	}
}

void ArmTeleopControl::End(bool interrupted) {
	arm->StopShoulder();
	arm->StopElbow();
}
