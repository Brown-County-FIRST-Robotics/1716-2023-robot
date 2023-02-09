#include "commands/AutoBalance.h"
#include "Constants.h"
#include <frc/smartdashboard/SmartDashboard.h>

AutoBalance::AutoBalance(Drivetrain* drive) 
	: CommandHelper{frc2::PIDController{PIDConst::PROPORTIONALFACTOR, PIDConst::INTEGRALFACTOR, PIDConst::DERIVATIVEFACTOR}, 
	[drive] { return drive->GetPitch(); },
	0,

	//[drive](double output) { drive->Drive(output, 0, 0); },
	[this, drive](double output) {frc::SmartDashboard::PutNumber("output", output);
							frc::SmartDashboard::PutNumber("Proportional", m_controller.GetP());
							frc::SmartDashboard::PutNumber("Integral", m_controller.GetI());
							frc::SmartDashboard::PutNumber("Derivative", m_controller.GetD());
							frc::SmartDashboard::PutNumber("Pitch", drive->GetPitch()); 
							drive->Drive(output, 0, 0); },

	{drive}},  //https://docs.wpilib.org/en/stable/docs/software/commandbased/pid-subsystems-commands.html#creating-a-pidcommand
	drivetrain(drive)
{
	m_controller.SetTolerance(-3, 3);
	m_controller.Reset();
	AddRequirements(drive);
	drivetrain->ActivateBreakMode(true);
}

void AutoBalance::End(bool interrupted) {
	//drivetrain->Drive(0, 0, 0);
	frc2::PIDCommand::End(interrupted); //call the original class's end function to not break anything within it
	drivetrain->ActivateBreakMode(false);
}