#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc/controller/PIDController.h>
#include <frc/Timer.h>
#include <frc/trajectory/Trajectory.h>
#include "subsystems/Drivetrain.h"

namespace TrajectoryConst{
	const double ROTATION_P=0;
	const double ROTATION_I=0;
	const double ROTATION_D=0;
	const double KV=0;
	const double KA=0;
};

class FollowTrajectory : public frc2::CommandHelper<frc2::CommandBase, FollowTrajectory> {
public:
	explicit FollowTrajectory(Drivetrain* drive, frc::Trajectory* traj);

	void Execute() override;
	
	bool IsFinished() override;

	void End(bool interrupted) override;

private:
	Drivetrain* drivetrain;
	frc::Trajectory* trajectory;
	bool hasStarted=false;
	frc::Timer splineTimer;
	frc2::PIDController rotationController{TrajectoryConst::ROTATION_P, TrajectoryConst::ROTATION_I, TrajectoryConst::ROTATION_D};
};



