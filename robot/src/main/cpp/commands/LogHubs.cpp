#include "commands/LogHubs.h"

#include <utility>

LogHubs::LogHubs(frc::PowerDistribution& pdh, frc::PneumaticHub& pch)
	: pdh{pdh}, pch{pch}
{
	frc::Shuffleboard::GetTab("Hub Log").Add("Compressor", compressor);
	compresserRunning = frc::Shuffleboard::GetTab("Hub Log").Add("Compressor Running", pch.GetCompressor()).GetEntry();
	
	frc::Shuffleboard::GetTab("Hub Log").Add("PDH", pdh);
}

void LogHubs::Execute() {
	compresserRunning->SetBoolean(pch.GetCompressor());
}

bool LogHubs::IsFinished() { return false; }

