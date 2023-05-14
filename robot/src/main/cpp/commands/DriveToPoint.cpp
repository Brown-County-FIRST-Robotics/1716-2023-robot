#include "commands/DriveToPoint.h"

#include <utility>

DriveToPoint::DriveToPoint(Drivetrain* drive, frc::Pose2d drive_to) 
	: drivetrain(drive), dest(drive_to)
{
	AddRequirements(drive);
	distPID.SetSetpoint(0);
	thetaPID.SetSetpoint(0);
	distPID.SetTolerance(AutoConst::DIST_TOL);
	thetaPID.SetTolerance(AutoConst::THETA_TOL);
}

DriveToPoint::DriveToPoint(Drivetrain* drive, const double pos[3])
	: drivetrain(drive), dest(pos[0]*1_m,pos[1]*1_m,frc::Rotation2d(pos[2]*1_deg))
{
	AddRequirements(drive);
	distPID.SetSetpoint(0);
	thetaPID.SetSetpoint(0);
	distPID.SetTolerance(AutoConst::DIST_TOL);
	thetaPID.SetTolerance(AutoConst::THETA_TOL);
}

void DriveToPoint::Execute() {
	auto pos = drivetrain->FetchPos();
	auto rot=pos.Rotation().Degrees().value()-dest.Rotation().Degrees().value();
	while(rot >= 360.0)
		rot -= 360.0;
	while (rot < 0.0)
		rot += 360.0;

	auto offset = pos-dest;
	auto ang = atan2(offset.Y().value(), offset.X().value());
	auto dist = sqrt(offset.X().value()*offset.X().value() + offset.Y().value()*offset.Y().value());
	auto distOut = distPID.Calculate(dist);

	drivetrain->Drive(distOut*cos(ang), distOut*sin(ang), thetaPID.Calculate(rot));
}

bool DriveToPoint::IsFinished() {
	return distPID.AtSetpoint() && thetaPID.AtSetpoint();
}


void DriveToPoint::End(bool interrupted) {
	drivetrain->Drive(0,0,0);
}
