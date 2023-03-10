#include "commands/AutoBalance.h"
#include "Constants.h"

AutoBalance::AutoBalance(Drivetrain* drive) 
	//https://docs.wpilib.org/en/stable/docs/software/commandbased/pid-subsystems-commands.html#creating-a-pidcommand
	: CommandHelper{frc2::PIDController{PIDConst::PROPORTIONAL_FACTOR, PIDConst::INTEGRAL_FACTOR, PIDConst::DERIVATIVE_FACTOR}, 
	[drive] { return drive->GetRoll(); }, //Input
	0, //Goal
	[drive](double output) { drive->Drive(output, 0, 0); }, //Output
	{drive}}, //Requirements
	drivetrain(drive)
{
	AddRequirements(drive);
	m_controller.SetTolerance(-3, 3);
	m_controller.Reset();
	drivetrain->ActivateBreakMode(true);
	drivetrain->SetSolenoid(frc::DoubleSolenoid::Value::kForward);
}

void AutoBalance::End(bool interrupted) {
	frc2::PIDCommand::End(interrupted); //call the original class's end function to not break anything within it
	drivetrain->Drive(0, 0, 0);
	drivetrain->ActivateBreakMode(false);
}