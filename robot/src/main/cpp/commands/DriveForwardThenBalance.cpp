#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/ParallelDeadlineGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/StartEndCommand.h>

#include "commands/DriveForwardThenBalance.h"

DriveForwardThenBalance::DriveForwardThenBalance(Drivetrain* subsystem)
{
	AddCommands(
		frc2::FunctionalCommand([subsystem] {subsystem->SetSolenoid(frc::DoubleSolenoid::Value::kReverse); subsystem->Drive(-1, 0, 0);}, []{}, [subsystem](bool i){subsystem->SetSolenoid(frc::DoubleSolenoid::Value::kForward);}, //command that backs up
			[subsystem]{return (fabs(subsystem->GetRoll()) > 10);}, {subsystem}), //until it is no longer level
		frc2::ParallelDeadlineGroup(
			frc2::WaitCommand(1_s),
		frc2::FunctionalCommand([subsystem] {subsystem->Drive(-1, 0, 0);}, []{}, [subsystem](bool i){subsystem->SetSolenoid(frc::DoubleSolenoid::Value::kForward);}, //command that backs up
			[subsystem]{return (fabs(subsystem->GetRoll()) > 20);}, {subsystem})), //until it is no longer level
		AutoBalance(subsystem)
	);
}