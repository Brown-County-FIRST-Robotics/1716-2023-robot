#include "commands/SpinTsrx.h"

SpinTsrx::SpinTsrx(Motors* subsystem) : motors(subsystem) {
	AddRequirements(subsystem);
}

void SpinTsrx::Initialize() {
	motors->SetTsrx(.7);
}

void SpinTsrx::End(bool interrupted) {
	motors->SetTsrx(0);
}

//bool SpinTfx::IsFinished() { } return whether it should finish