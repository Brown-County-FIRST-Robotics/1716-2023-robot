#include "commands/SpinTsrx.h"

SpinTsrx::SpinTsrx(Motors* subsystem){
	AddRequirements(subsystem);
	motors = new Motors();
}

void SpinTsrx::Initialize() {
	motors->SetTsrx(.7);
}

void SpinTsrx::End(bool interrupted) {
	motors->SetTsrx(0);
}

//bool SpinTfx::IsFinished() { } return whether it should finish