#pragma once
#include <iostream>

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
// #include <networktables/FloatArrayTopic.h>
#include <networktables/FloatTopic.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

#include "subsystems/Drivetrain.h"

class RasPiDrive : public frc2::CommandHelper<frc2::CommandBase, RasPiDrive> {
public:
	explicit RasPiDrive(Drivetrain* drive);
	
	void Execute() override;

	void End(bool interrupted) override;

private:
	Drivetrain* drivetrain;

	nt::NetworkTableInstance networkTableInst; //the default networktables network
	std::shared_ptr<nt::NetworkTable> drive_table; //the table for driving
	std::shared_ptr<nt::NetworkTable> encoder_table; //the table for driving


	nt::FloatSubscriber x; //x, y, and rotational values
	nt::FloatSubscriber y;
	nt::FloatSubscriber z;

	nt::FloatPublisher fl_encoder;
	nt::FloatPublisher bl_encoder;
	nt::FloatPublisher fr_encoder;
	nt::FloatPublisher br_encoder;


	nt::FloatPublisher yaw;
	nt::FloatPublisher forwBack;
	nt::FloatPublisher leftRight;
};