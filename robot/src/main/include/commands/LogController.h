#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc/controller/PIDController.h>
#include <frc2/command/button/CommandXboxController.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/BooleanTopic.h>
#include <frc/shuffleboard/Shuffleboard.h>


class LogController : public frc2::CommandHelper<frc2::CommandBase, LogController> {
public:
	explicit LogController(frc2::CommandXboxController &controller);

	void Execute() override;
	
	bool IsFinished() override;

private:
	nt::GenericEntry* a;
	nt::GenericEntry* b;
	nt::GenericEntry* x;
	nt::GenericEntry* y;
	nt::GenericEntry* lb;
	nt::GenericEntry* rb;
	nt::GenericEntry* lt;
	nt::GenericEntry* rt;
	nt::GenericEntry* pov;
	nt::GenericEntry* back;
	nt::GenericEntry* start;
	nt::GenericEntry* lx;
	nt::GenericEntry* ly;
	nt::GenericEntry* rx;
	nt::GenericEntry* ry;
	nt::GenericEntry* ls;
	nt::GenericEntry* rs;
	frc2::CommandXboxController& controller;
};