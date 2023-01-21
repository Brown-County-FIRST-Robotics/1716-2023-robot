#include "commands/AutoBalance.h"
#include "Constants.h"

AutoBalance::AutoBalance(Drivetrain* subsystem) : drivetrain(subsystem)
{
	AddRequirements(subsystem);
}

void AutoBalance::Execute() {
	pitch = drivetrain->GetPitch();
	integralElement += pitch;

	proportional = pitch * PIDConst::PROPORTIONALFACTOR;
	integral = integralElement * PIDConst::INTEGRALFACTOR;
	derivative = (pitch-prevPos) * PIDConst::DERIVATIVEFACTOR;

	drivetrain->Drive(proportional + integral + derivative, 0, 0);
	prevPos = pitch;
}

void AutoBalance::End(bool interrupted) {
	drivetrain->Drive(0, 0, 0);
}