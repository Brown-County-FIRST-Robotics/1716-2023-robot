#pragma once

#include <frc2/command/CommandHelper.h>
#include <frc2/command/SequentialCommandGroup.h>

#include "subsystems/Drivetrain.h"
#include "subsystems/Arm.h"
#include "commands/AutoBalance.h"


class PlaceThenMobility : public frc2::CommandHelper<frc2::SequentialCommandGroup, PlaceThenMobility> {
public:
	explicit PlaceThenMobility(Drivetrain* drive, Arm* arm);
};