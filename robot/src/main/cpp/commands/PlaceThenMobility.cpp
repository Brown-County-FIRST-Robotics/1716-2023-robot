#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/ParallelDeadlineGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/StartEndCommand.h>

#include "commands/PlaceThenMobility.h"

PlaceThenMobility::PlaceThenMobility(Drivetrain* drive, Arm* arm)
{
	AddCommands(
		frc2::FunctionalCommand(
			[arm] {
				arm->SetElbowGoal(ArmHeightConst::HIGHCUBE[1]);
				arm->SetShoulderGoal(ArmHeightConst::HIGHCUBE[0]);
				arm->SetClaw(ArmConst::CLAW_CLOSED);
			}, 
			[]{}, // No execute function 
			[arm](bool interupted){
			}, // End function
			[arm]{
				return fabs(arm->GetElbowGoal()-arm->GetElbowPosition())
				+fabs(arm->GetShoulderGoal()-arm->GetShoulderPosition())<ArmHeightConst::THRESHOLD;
				}, // IsFinished
			{arm} // requirenents
		),
		frc2::FunctionalCommand(
			[arm] {
				arm->SetClaw(ArmConst::CLAW_OPEN);
				arm->InitiateDrivePreset();
			}, 
			[]{}, // No execute function 
			[arm](bool interupted){
			}, // End function
			[arm]{
				return fabs(arm->GetElbowGoal()-arm->GetElbowPosition())
				+fabs(arm->GetShoulderGoal()-arm->GetShoulderPosition())<ArmHeightConst::THRESHOLD;
				}, // IsFinished
			{arm} // requirenents
		),
		frc2::ParallelDeadlineGroup(
			frc2::WaitCommand(5_s),
			frc2::StartEndCommand([drive] {drive->SetSolenoid(frc::DoubleSolenoid::Value::kReverse); drive->Drive(-0.2, 0, 0);}, 
				[drive] { drive->Drive(0, 0, 0); }, {drive}) //command that backs up
		)
	);
} 	