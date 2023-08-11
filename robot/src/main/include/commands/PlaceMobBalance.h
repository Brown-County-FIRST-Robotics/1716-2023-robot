#pragma once

#include <frc2/command/CommandHelper.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc/trajectory/TrajectoryConfig.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc2/command/MecanumControllerCommand.h>

#include "subsystems/Drivetrain.h"
#include "subsystems/Arm.h"
#include "commands/AutoBalance.h"
#include "commands/DriveForwardThenBalance.h"


class PlaceMobBalance : public frc2::CommandHelper<frc2::SequentialCommandGroup, PlaceMobBalance> {
public:
	explicit PlaceMobBalance(Drivetrain* drive, Arm* arm, bool cable_bump=false);
};