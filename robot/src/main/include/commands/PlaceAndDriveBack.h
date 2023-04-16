#pragma once

#include <frc2/command/CommandHelper.h>
#include <frc2/command/SequentialCommandGroup.h>

#include "subsystems/Drivetrain.h"
#include "subsystems/Arm.h"
#include "commands/AutoBalance.h"
#include "commands/DriveForwardThenBalance.h"


class PlaceAndDriveBack : public frc2::CommandHelper<frc2::SequentialCommandGroup, PlaceAndDriveBack> {
public:
	explicit PlaceAndDriveBack(Drivetrain* drive, Arm* arm);
};