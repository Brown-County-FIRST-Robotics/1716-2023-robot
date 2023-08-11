#pragma once

#include <frc2/command/Command.h>
#include <frc2/command/Commands.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SequentialCommandGroup.h>

#include "subsystems/LED.h"
#include "subsystems/Drivetrain.h"
#include "subsystems/Arm.h"
#include "commands/DriveToPoint.h"


class PlacePiece : public frc2::CommandHelper<frc2::SequentialCommandGroup, PlacePiece> {
public:
	explicit PlacePiece(Drivetrain* drive, Arm* arm, LED* led, int height, int col);
};
