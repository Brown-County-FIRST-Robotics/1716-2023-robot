#include "commands/DriveCartesian.h"

#include <utility>

DriveCartesian::DriveCartesian(Drivetrain* subsystem, std::function<double()> forward, std::function<double()> right, std::function<double()> rotation) 
	: drivetrain(subsystem), x(std::move(forward)), y(std::move(right)), z(std::move(rotation)) 
{
	AddRequirements(subsystem);
}

void DriveCartesian::Execute() {
	drivetrain->Drive(-x(), y(), z());
}

void DriveCartesian::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}