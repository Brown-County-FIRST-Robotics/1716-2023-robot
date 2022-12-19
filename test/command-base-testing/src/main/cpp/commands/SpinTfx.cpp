#include "commands/SpinTfx.h"

SpinTfx::SpinTfx(Motors* subsystem) : motors(subsystem) {
	AddRequirements(subsystem);
}

void SpinTfx::Initialize() {
	motors->SetTfx(.7);
}

void SpinTfx::End(bool interrupted) {
	motors->SetTfx(0);
}

//bool SpinTfx::IsFinished() { } return whether it should finish