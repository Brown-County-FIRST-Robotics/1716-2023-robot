#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/ParallelDeadlineGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/StartEndCommand.h>

#include "commands/DriveBackThenBalance.h"

DriveBackThenBalance::DriveBackThenBalance(Drivetrain* subsystem)
{
	AddCommands(
		frc2::FunctionalCommand([subsystem] {subsystem->SetSolenoid(frc::DoubleSolenoid::Value::kForward); subsystem->Drive(-.3, 0, 0);}, []{}, [](bool i){}, //command that backs up
			[subsystem]{return (fabs(subsystem->GetRoll()) > 3);}, {subsystem}), //until it is no longer level
		// frc2::ParallelDeadlineGroup(frc2::FunctionalCommand([]{}, []{}, [](bool i){}, [&subsystem] { return fabs(subsystem->GetPitch()) < 3; }),
		// 	AutoBalance(subsystem)), //auto balance until level, decorators don't work in this
		// frc2::ParallelDeadlineGroup(frc2::WaitCommand(0.5_s),
		// 	frc2::StartEndCommand([&subsystem] { subsystem->Drive(-.2, 0, 0); }, [&subsystem]{ subsystem->Drive(0, 0, 0); })),
		AutoBalance(subsystem)
	);
}