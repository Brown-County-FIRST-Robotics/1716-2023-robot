#pragma once

#include <frc2/command/CommandHelper.h>
#include <frc2/command/SequentialCommandGroup.h>

#include "subsystems/Drivetrain.h"
#include "commands/Autobalance.h"

class DriveForwardThenBalance : public frc2::CommandHelper<frc2::SequentialCommandGroup, DriveForwardThenBalance> {
public:
	explicit DriveForwardThenBalance(Drivetrain* drive);
};