#include <span>

#include "commands/RasPiDrive.h"
#include "Constants.h"

RasPiDrive::RasPiDrive(Drivetrain* drive) : drivetrain(drive) {
	AddRequirements(drive);

	networkTableInst = nt::NetworkTableInstance::GetDefault();
	table = networkTableInst.GetTable("1716Drive");

	x = table->GetFloatTopic("x").Subscribe(0.0, {.pollStorage = 1}); //set the x, y, and rotation listeners 
	y = table->GetFloatTopic("y").Subscribe(0.0, {.pollStorage = 1}); //with a default of 0 and a memory of 1 term
	z = table->GetFloatTopic("rotation").Subscribe(0.0, {.pollStorage = 1});

	forwBack = table->GetFloatTopic("xAccel").Publish();
	leftRight = table->GetFloatTopic("yAccel").Publish();
	yaw = table->GetFloatTopic("yaw").Publish();
}

void RasPiDrive::Execute() {
	drivetrain->Drive(x.Get(), y.Get(), z.Get());

	yaw.Set(drivetrain->GetYaw());
	forwBack.Set(drivetrain->GetY());
	leftRight.Set(drivetrain->GetX());
}

void RasPiDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}