#include <span>

#include "commands/RasPiDrive.h"
#include "Constants.h"

RasPiDrive::RasPiDrive(Drivetrain* drive) : drivetrain(drive) {
	AddRequirements(drive);

	networkTableInst = nt::NetworkTableInstance::GetDefault();
	driveTable = networkTableInst.GetTable("1716Drive");
	encoderTable = networkTableInst.GetTable("1716Encoder");	
	pigeonTable = networkTableInst.GetTable("1716Pigeon");

	x = driveTable->GetFloatTopic("x").Subscribe(0.0, {.pollStorage = 1}); //set the x, y, and rotation listeners 
	y = driveTable->GetFloatTopic("y").Subscribe(0.0, {.pollStorage = 1}); //with a default of 0 and a memory of 1 term
	z = driveTable->GetFloatTopic("rotation").Subscribe(0.0, {.pollStorage = 1});

	flEncoder = encoderTable->GetFloatTopic("frontLeftEncoder").Publish();
	blEncoder = encoderTable->GetFloatTopic("backLeftEncoder").Publish();
	frEncoder = encoderTable->GetFloatTopic("frontRightEncoder").Publish();
	brEncoder = encoderTable->GetFloatTopic("backRightEncoder").Publish();
	resetEncoders = encoderTable->GetBooleanTopic("resetEncoder").GetEntry(false);

	xAccel = driveTable->GetFloatTopic("xAccel").Publish();
	yAccel = driveTable->GetFloatTopic("yAccel").Publish();
	zAccel = driveTable->GetFloatTopic("zAccel").Publish();
	yaw = driveTable->GetFloatTopic("yaw").Publish();
}

void RasPiDrive::Execute() {
	drivetrain->Drive(x.Get(), y.Get(), z.Get());

	if (resetEncoders.Get()) {
		drivetrain->ResetEncoders();
		resetEncoders.Set(false);
	}

	flEncoder.Set(drivetrain->GetEncoder(DrivetrainConst::FRONTLEFTID));
	blEncoder.Set(drivetrain->GetEncoder(DrivetrainConst::BACKLEFTID));
	frEncoder.Set(drivetrain->GetEncoder(DrivetrainConst::FRONTRIGHTID));
	brEncoder.Set(drivetrain->GetEncoder(DrivetrainConst::BACKRIGHTID));

	yaw.Set(drivetrain->GetYaw());
	yAccel.Set(drivetrain->GetY());
	xAccel.Set(drivetrain->GetX());
	zAccel.Set(drivetrain->GetZ());

}

void RasPiDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}