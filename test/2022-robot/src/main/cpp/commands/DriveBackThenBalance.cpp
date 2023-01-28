#include <frc2/command/FunctionalCommand.h>

#include "commands/DriveBackThenBalance.h"

DriveBackThenBalance::DriveBackThenBalance(Drivetrain* subsystem)
{
	AddCommands(
		frc2::FunctionalCommand([subsystem] {subsystem->Drive(-.2, 0, 0);}, []{}, [](bool i){}, //command that backs up
			[subsystem]{return (fabs(subsystem->GetPitch()) > 3);}, {subsystem}), //until it is no longer level
		AutoBalance(subsystem) //it then auto-balances
	);
}