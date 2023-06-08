#include "commands/FollowTrajectory.h"

#include <utility>

FollowTrajectory::FollowTrajectory(Drivetrain* drive, frc::Trajectory* traj) : drivetrain(drive), trajectory(traj)
{
	AddRequirements(drive);
}


void FollowTrajectory::Execute() {
	if(!hasStarted){
		splineTimer.Start();
		hasStarted=true;
	}
	auto pos = drivetrain->FetchPos();
	auto p2 = trajectory->Sample(splineTimer.Get());
	auto pursuitAng=atan2(pos.Y().value()-p2.pose.Y().value(), pos.X().value()-p2.pose.X().value());
	auto xVelocity=p2.velocity.value()*cos(pursuitAng);
	auto yVelocity=p2.velocity.value()*sin(pursuitAng);
	auto xAccel=p2.acceleration.value()*cos(pursuitAng);
	auto yAccel=p2.acceleration.value()*sin(pursuitAng);

	auto thetaDiff=(p2.pose-pos).Rotation().Degrees().value();
	while(thetaDiff>180){
		thetaDiff-=360;
	}
	drivetrain->Drive(
		xVelocity*TrajectoryConst::KV+xAccel*TrajectoryConst::KA,
		yVelocity*TrajectoryConst::KV+yAccel*TrajectoryConst::KA,
		rotationController.Calculate(thetaDiff)
	);

}

bool FollowTrajectory::IsFinished() {
	return splineTimer.Get().value()>trajectory->TotalTime().value();
}


void FollowTrajectory::End(bool interrupted) {
	drivetrain->Drive(0,0,0);
	splineTimer.Stop();
	splineTimer.Reset();
	hasStarted=false;
}
