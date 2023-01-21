#include "commands/SpinNeo.h"

#include <utility>
#include <frc/smartdashboard/SmartDashboard.h>

/*SpinNeo::SpinNeo(Motors* subsystem) : motors(subsystem) {
	AddRequirements(subsystem);
}*/

SpinNeo::SpinNeo(Neo *subsystem, std::function<double()> speed_a) : neo(subsystem), speed(std::move(speed_a)) {
	AddRequirements(subsystem);
}

void SpinNeo::Initialize() {
	//motors->SetNeo(.7);
}

void SpinNeo::Execute() {
	neo->SetNeo(speed());
}

void SpinNeo::End(bool interrupted) {
	neo->SetNeo(0);
}

//bool SpinTfx::IsFinished() { } return whether it should finish