#include "commands/DriveToPoint.h"

#include <utility>

DriveToPoint::DriveToPoint(Drivetrain* drive, frc::Pose2d drive_to) 
	: drivetrain(drive), dest(drive_to)
{
	AddRequirements(drive);
	xPID.SetSetpoint(dest.X().value());
	yPID.SetSetpoint(dest.Y().value());
	thetaPID.SetSetpoint(0);
	xPID.SetTolerance(AutoConst::X_TOL);
	yPID.SetTolerance(AutoConst::Y_TOL);
	thetaPID.SetTolerance(AutoConst::THETA_TOL);
}

DriveToPoint::DriveToPoint(Drivetrain* drive, const double pos[3])
	: drivetrain(drive), dest(pos[0]*1_m,pos[1]*1_m,frc::Rotation2d(pos[2]*1_deg))
{
	AddRequirements(drive);
	xPID.SetSetpoint(dest.X().value());
	yPID.SetSetpoint(dest.Y().value());
	thetaPID.SetSetpoint(0);
	xPID.SetTolerance(AutoConst::X_TOL);
	yPID.SetTolerance(AutoConst::Y_TOL);
	thetaPID.SetTolerance(AutoConst::THETA_TOL);
}

void DriveToPoint::Execute() {
	auto pos = drivetrain->FetchPos();
	auto rot=pos.Rotation().Degrees().value()-dest.Rotation().Degrees().value();
	while(rot >= 360.0)
		rot -= 360.0;
	while (rot < 0.0)
		rot += 360.0;

	drivetrain->Drive(yPID.Calculate(pos.Y().value()), xPID.Calculate(pos.X().value()), thetaPID.Calculate(rot));
}

bool DriveToPoint::IsFinished() {
	return xPID.AtSetpoint() && yPID.AtSetpoint() && thetaPID.AtSetpoint();
}


void DriveToPoint::End(bool interrupted) {
	drivetrain->Drive(0,0,0);
}
