#include "commands/SpinAll.h"

SpinAll::SpinAll(Motors* subsystem){
	AddRequirements(subsystem);
	motors = new Motors();
}

void SpinAll::Initialize() {
	motors->SetTfx(.7);
	motors->SetTsrx(.7);
}

void SpinAll::End(bool interrupted) {
	motors->SetTfx(0);
	motors->SetTsrx(0);
}

//bool SpinTfx::IsFinished() { } return whether it should finish