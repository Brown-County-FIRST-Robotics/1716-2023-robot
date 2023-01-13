#pragma once

#include <frc2/command/CommandHelper.h>
#include <frc2/command/SequentialCommandGroup.h>

#include "SpinTfx.h"
#include "SpinTsrx.h"
#include "SpinAll.h"

class SpinSrxThenFxThenBoth : public frc2::CommandHelper<frc2::SequentialCommandGroup, SpinSrxThenFxThenBoth> {
public:
	SpinSrxThenFxThenBoth(Motors* subsystem);
};