#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/Arm.h"

class ArmTeleopControl : public frc2::CommandHelper<frc2::CommandBase, ArmTeleopControl> {
public:
	explicit ArmTeleopControl(Arm* arm,
		std::function<int()> elbowPov, //takes a DPad
		std::function<bool()> clawBool,
		std::function<bool()> floorBool,
		std::function<bool()> mediumBool,
		std::function<bool()> highBool,
		std::function<bool()> driveBool,
		std::function<bool()> portalBool);

	void Execute() override;
	
	void End(bool interrupted) override;

private:
	Arm* arm;

	std::function<int()> elbow;
	std::function<bool()> claw;

	bool elbowStopped = false;

	
//TEMP CODE: MANUAL SHOULDER CONTROL
	bool shoulderStopped = false;
//END TEMP CODE: MANUAL SHOULDER CONTROL


	bool clawPressed = false;
	
	//Presets
//TEMP CODE: SHUFFLEBOARD PRESETS
	// nt::GenericEntry* high;
	// nt::GenericEntry* medium;
	// nt::GenericEntry* floor;
	// nt::GenericEntry* portal;
	// nt::GenericEntry* drive;
//END TEMP CODE: SHUFFLEBOARD PRESETS

	std::function<bool()> floorButton;
	std::function<bool()> mediumButton;
	std::function<bool()> highButton;
	std::function<bool()> portalButton;
	std::function<bool()> driveButton;

	bool floorButtonPrevState = false;
	bool mediumButtonPrevState = false;
	bool highButtonPrevState = false;
	bool portalButtonPrevState = false;
	bool driveButtonPrevState = false;
};