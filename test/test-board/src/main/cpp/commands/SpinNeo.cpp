#include "commands/SpinNeo.h"

SpinNeo::SpinNeo(Motors* subsystem) : motors(subsystem) {
	AddRequirements(subsystem);
}

void SpinNeo::Initialize() {
	motors->SetNeo(.7);
}

void SpinNeo::End(bool interrupted) {
	motors->SetNeo(0);
}

//bool SpinTfx::IsFinished() { } return whether it should finish