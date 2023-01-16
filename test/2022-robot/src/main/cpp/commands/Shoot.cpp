#include "commands/Shoot.h"

#include <utility>

Shoot::Shoot(Arm* subsystem/*, std::function<double()> speed_a*/) : arm(subsystem)/*, speed(std::move(speed_a))*/ {
	AddRequirements(subsystem);
}

void Shoot::Execute() {
	arm->SetShooter(0.7);//speed());
}

void Shoot::End(bool interrupted) {
	arm->SetShooter(0);
}

//bool SpinTfx::IsFinished() { } return whether it should finish