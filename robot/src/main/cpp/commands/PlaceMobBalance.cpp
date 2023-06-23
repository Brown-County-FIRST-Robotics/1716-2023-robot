#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/ParallelDeadlineGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/StartEndCommand.h>

#include "commands/PlaceMobBalance.h"

PlaceMobBalance::PlaceMobBalance(Drivetrain* drive, Arm* arm)
{
	frc::TrajectoryConfig conf{TrajectoryFollowingConst::MAX_VELOCITY, TrajectoryFollowingConst::MAX_ACCELERATION};
	frc::Trajectory traj=frc::TrajectoryGenerator::GenerateTrajectory(frc::Pose2d(14.91_m,1.22_m,0_deg),{frc::Translation2d(14.91_m,1_m),frc::Translation2d(14.91_m,1_m)},frc::Pose2d(14.91_m,3_m,0_deg), conf);
	AddCommands(
		/*frc2::FunctionalCommand(
			[arm] {
				arm->SetElbowGoal(ArmHeightConst::HIGHCUBE[1]);
				arm->SetShoulderGoal(ArmHeightConst::HIGHCUBE[0]);
				arm->SetClaw(ArmConst::CLAW_CLOSED);
			}, 
			[]{}, // No execute function 
			[](bool interupted){}, // End function
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
			[](bool interupted){}, // End function
			[arm]{
				return fabs(arm->GetElbowGoal()-arm->GetElbowPosition())
				+fabs(arm->GetShoulderGoal()-arm->GetShoulderPosition())<ArmHeightConst::THRESHOLD;
				}, // IsFinished
			{arm} // requirenents
		),*/
		frc2::MecanumControllerCommand(
			traj,
			[drive]() { return drive->FetchPos(); },
			frc::SimpleMotorFeedforward<units::meters>(TrajectoryFollowingConst::FEEDFORWARD_GAIN, TrajectoryFollowingConst::FEEDFORWARD_VELCOITY, TrajectoryFollowingConst::FEEDFORWARD_ACCELERATION),
			drive->kinematics,

			frc2::PIDController{TrajectoryFollowingConst::X_P, TrajectoryFollowingConst::X_I, TrajectoryFollowingConst::X_D}, // x controller
			frc2::PIDController{TrajectoryFollowingConst::Y_P, TrajectoryFollowingConst::Y_I, TrajectoryFollowingConst::Y_D}, // y controller
			frc::ProfiledPIDController<units::radians>(
				TrajectoryFollowingConst::THETA_P, TrajectoryFollowingConst::THETA_I, TrajectoryFollowingConst::THETA_D,
				TrajectoryFollowingConst::ROTATION_CONSTRAINTS
			), // theta controller

			TrajectoryFollowingConst::MAX_VELOCITY, // max velocity
			[drive]() {
				return drive->GetEncoderSpeeds();
			},
			frc2::PIDController{TrajectoryFollowingConst::FL_P, TrajectoryFollowingConst::FL_I, TrajectoryFollowingConst::FL_D},  // FrontLeft
			frc2::PIDController{TrajectoryFollowingConst::BL_P, TrajectoryFollowingConst::BL_I, TrajectoryFollowingConst::BL_D},  // RearLeft
			frc2::PIDController{TrajectoryFollowingConst::FR_P, TrajectoryFollowingConst::FR_I, TrajectoryFollowingConst::FR_D},  // FrontRight
			frc2::PIDController{TrajectoryFollowingConst::BR_P, TrajectoryFollowingConst::BR_I, TrajectoryFollowingConst::BR_D},  // RearRight
			[drive](units::volt_t frontLeft, units::volt_t rearLeft, units::volt_t frontRight, units::volt_t rearRight) {
				drive->DriveVolts({frontLeft, rearLeft, frontRight, rearRight});
			},
			{drive}
		),
		DriveForwardThenBalance(drive, 1)
	);
}