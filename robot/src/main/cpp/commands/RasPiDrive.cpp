#include <span>

#include "commands/RasPiDrive.h"
#include "Constants.h"

RasPiDrive::RasPiDrive(Drivetrain* drive) : drivetrain(drive) {
	AddRequirements(drive);

	networkTableInst = nt::NetworkTableInstance::GetDefault();
	drive_table = networkTableInst.GetTable("1716Drive");
	encoder_table=networkTableInst.GetTable("1716Encoder");	

	x = drive_table->GetFloatTopic("x").Subscribe(0.0, {.pollStorage = 1}); //set the x, y, and rotation listeners 
	y = drive_table->GetFloatTopic("y").Subscribe(0.0, {.pollStorage = 1}); //with a default of 0 and a memory of 1 term
	z = drive_table->GetFloatTopic("rotation").Subscribe(0.0, {.pollStorage = 1});

	fl_encoder=encoder_table->GetFloatTopic("frontLeftEncoder").Publish();
	bl_encoder=encoder_table->GetFloatTopic("backLeftEncoder").Publish();
	fr_encoder=encoder_table->GetFloatTopic("frontRightEncoder").Publish();
	br_encoder=encoder_table->GetFloatTopic("backRightEncoder").Publish();


	forwBack = drive_table->GetFloatTopic("xAccel").Publish();
	leftRight = drive_table->GetFloatTopic("yAccel").Publish();
	yaw = drive_table->GetFloatTopic("yaw").Publish();
}

void RasPiDrive::Execute() {
	drivetrain->Drive(x.Get(), y.Get(), z.Get());

	if(encoder_table->GetBoolean("resetEncoder",false)){ // TODO: probably change
		drivetrain->ResetEncoders();
		encoder_table->PutBoolean("resetEncoder",false);
		std::cout << "reset\n\n\n\n\n\n\n\n\n\n\n\n";
	}
	fl_encoder.Set(drivetrain->GetEncoder(DrivetrainConst::FRONTLEFTID));
	bl_encoder.Set(drivetrain->GetEncoder(DrivetrainConst::BACKLEFTID));
	fr_encoder.Set(drivetrain->GetEncoder(DrivetrainConst::FRONTRIGHTID));
	br_encoder.Set(drivetrain->GetEncoder(DrivetrainConst::BACKRIGHTID));

	//std::cout << drivetrain->GetEncoder(DrivetrainConst::FRONTLEFTID) << " " <<
	//drivetrain->GetEncoder(DrivetrainConst::FRONTRIGHTID) << " " <<
	//drivetrain->GetEncoder(DrivetrainConst::BACKLEFTID) << " " <<
	//drivetrain->GetEncoder(DrivetrainConst::BACKRIGHTID) << " " << "\n";


	yaw.Set(drivetrain->GetYaw());
	forwBack.Set(drivetrain->GetY());
	leftRight.Set(drivetrain->GetX());
}

void RasPiDrive::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}