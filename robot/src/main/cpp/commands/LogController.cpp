#include "commands/LogController.h"

#include <utility>

LogController::LogController(frc2::CommandXboxController &controller) 
	: controller{controller}
{
	a = frc::Shuffleboard::GetTab("Controller").Add("A", controller.GetAButton()).GetEntry();
	b = frc::Shuffleboard::GetTab("Controller").Add("B", controller.GetBButton()).GetEntry();
	x = frc::Shuffleboard::GetTab("Controller").Add("X", controller.GetXButton()).GetEntry();
	y = frc::Shuffleboard::GetTab("Controller").Add("Y", controller.GetYButton()).GetEntry();
	lb = frc::Shuffleboard::GetTab("Controller").Add("Left Bumper", controller.GetLeftBumper()).GetEntry();
	rb = frc::Shuffleboard::GetTab("Controller").Add("Right Bumper", controller.GetRightBumper()).GetEntry();
	lt = frc::Shuffleboard::GetTab("Controller").Add("Left Trigger", controller.GetLeftTriggerAxis()).GetEntry();
	rt = frc::Shuffleboard::GetTab("Controller").Add("Right Trigger", controller.GetRightTriggerAxis()).GetEntry();
	pov = frc::Shuffleboard::GetTab("Controller").Add("POV", controller.GetPOV()).GetEntry();
	back = frc::Shuffleboard::GetTab("Controller").Add("Back", controller.GetBackButton()).GetEntry();
	start = frc::Shuffleboard::GetTab("Controller").Add("Start", controller.GetStartButton()).GetEntry();
	lx = frc::Shuffleboard::GetTab("Controller").Add("Left X", controller.GetLeftX()).GetEntry();
	ly = frc::Shuffleboard::GetTab("Controller").Add("Left Y", -controller.GetLeftY()).GetEntry();
	rx = frc::Shuffleboard::GetTab("Controller").Add("Right X", controller.GetRightX()).GetEntry();
	ry = frc::Shuffleboard::GetTab("Controller").Add("Right Y", -controller.GetRightY()).GetEntry();
	ls = frc::Shuffleboard::GetTab("Controller").Add("Left Stick Button", controller.GetLeftStickButton()).GetEntry();
	rs = frc::Shuffleboard::GetTab("Controller").Add("Right Stick Button", controller.GetRightStickButton()).GetEntry();
}

void LogController::Execute() {
	a->SetBoolean(controller.GetAButton());
	b->SetBoolean(controller.GetBButton());
	x->SetBoolean(controller.GetXButton());
	y->SetBoolean(controller.GetYButton());
	lb->SetBoolean(controller.GetLeftBumper());
	rb->SetBoolean(controller.GetRightBumper());
	lt->SetDouble(controller.GetLeftTriggerAxis());
	rt->SetDouble(controller.GetRightTriggerAxis());
	pov->SetInteger(controller.GetPOV());
	back->SetBoolean(controller.GetBackButton());
	start->SetBoolean(controller.GetStartButton());
	lx->SetDouble(controller.GetLeftX());
	ly->SetDouble(-controller.GetLeftY());
	rx->SetDouble(controller.GetRightX());
	ry->SetDouble(-controller.GetRightY());
	ls->SetBoolean(controller.GetLeftStickButton());
	rs->SetBoolean(controller.GetRightStickButton());
}

bool LogController::IsFinished() { return false; }


