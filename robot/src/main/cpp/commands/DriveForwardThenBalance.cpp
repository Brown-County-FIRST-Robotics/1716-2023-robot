#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/ParallelDeadlineGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/StartEndCommand.h>

#include "commands/DriveForwardThenBalance.h"

DriveForwardThenBalance::DriveForwardThenBalance(Drivetrain* drive, double direction)
{
	AddCommands(
		frc2::FunctionalCommand([drive, direction] {drive->SetSolenoid(frc::DoubleSolenoid::Value::kReverse); drive->Drive(direction, 0, 0);}, []{}, [drive](bool i){drive->SetSolenoid(frc::DoubleSolenoid::Value::kForward);}, //command that backs up
			[drive]{return (fabs(drive->GetRoll()) > 10);}, {drive}), //until it is no longer level
		frc2::ParallelDeadlineGroup(
			frc2::WaitCommand(1_s),
		frc2::FunctionalCommand([drive, direction] {drive->Drive(direction, 0, 0);}, []{}, [drive](bool i){drive->SetSolenoid(frc::DoubleSolenoid::Value::kForward);}, //command that backs up
			[drive]{return (fabs(drive->GetRoll()) > 20);}, {drive})), //until it is no longer level
		AutoBalance(drive)
	);
}