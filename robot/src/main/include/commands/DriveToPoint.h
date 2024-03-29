#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc/controller/PIDController.h>
#include "subsystems/Drivetrain.h"


class DriveToPoint : public frc2::CommandHelper<frc2::CommandBase, DriveToPoint> {
public:
	explicit DriveToPoint(Drivetrain* drive, frc::Pose2d drive_to);
	explicit DriveToPoint(Drivetrain* drive, const double drive_to[3]);

	void Execute() override;
	
	bool IsFinished() override;

	void End(bool interrupted) override;

private:
	Drivetrain* drivetrain;
	frc::Pose2d dest;
	frc::PIDController distPID{AutoConst::DIST_PID[0], AutoConst::DIST_PID[1], AutoConst::DIST_PID[2]};
	frc::PIDController thetaPID{AutoConst::THETA_PID[0], AutoConst::THETA_PID[1], AutoConst::THETA_PID[2]};
};