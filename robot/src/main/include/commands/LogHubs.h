#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc/controller/PIDController.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/BooleanTopic.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/Compressor.h>
#include <frc/PowerDistribution.h>
#include <frc/PneumaticHub.h>



class LogHubs : public frc2::CommandHelper<frc2::CommandBase, LogHubs> {
public:
	explicit LogHubs(frc::PowerDistribution& pdh, frc::PneumaticHub& pch);

	void Execute() override;
	
	bool IsFinished() override;

private:
	frc::PowerDistribution& pdh;
	frc::PneumaticHub& pch;
	nt::GenericEntry* compresserRunning;
	frc::Compressor compressor=pch.MakeCompressor();
};