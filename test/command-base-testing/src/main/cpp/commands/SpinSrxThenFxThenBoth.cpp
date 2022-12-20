#include "commands/SpinSrxThenFxThenBoth.h"

#include <units/time.h>

SpinSrxThenFxThenBoth::SpinSrxThenFxThenBoth(Motors* subsystem) {
	AddCommands(SpinTsrx{subsystem}.WithTimeout(2_s), SpinTfx{subsystem}.WithTimeout(2_s), SpinAll{subsystem}.WithTimeout(2_s));
}