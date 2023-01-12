#include "commands/SpinMotor.h"

#include <utility>
#include <frc/smartdashboard/SmartDashboard.h>

SpinMotor::SpinMotor(Motor* subsystem, std::function<double()> speed_a) : motor(subsystem), speed(std::move(speed_a)) {
	AddRequirements(subsystem);
}

void SpinMotor::Execute() {
	motor->Set(speed());
	frc::SmartDashboard::PutNumber("Right Trigger Value", speed());
}

void SpinMotor::End(bool interrupted) {
	motor->Set(0);
}

//bool SpinTfx::IsFinished() { } return whether it should finish