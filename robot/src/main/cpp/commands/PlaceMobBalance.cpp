#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/ParallelDeadlineGroup.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/StartEndCommand.h>
#include <frc/DriverStation.h>

#include "commands/PlaceMobBalance.h"

frc::Pose2d convert(frc::Pose2d pos){
	if(frc::DriverStation::GetAlliance()==frc::DriverStation::Alliance::kBlue)
		return frc::Pose2d(325.61_in-pos.X(),pos.Y(),180_deg-pos.Rotation().Degrees());
	return pos;
}

frc::Translation2d convert(frc::Translation2d pos){
	if(frc::DriverStation::GetAlliance()==frc::DriverStation::Alliance::kBlue)
		return frc::Translation2d(325.61_in-pos.X(),pos.Y());
	return pos;
}

PlaceMobBalance::PlaceMobBalance(Drivetrain* drive, Arm* arm,bool cable_bump)
{
	frc::TrajectoryConfig conf{TrajectoryFollowingConst::MAX_VELOCITY, TrajectoryFollowingConst::MAX_ACCELERATION};
	conf.SetEndVelocity(1_m / 1_s);

	frc::Trajectory traj=frc::TrajectoryGenerator::GenerateTrajectory(convert(frc::Pose2d(14.91_m,cable_bump?1.22_m:4.4_m,0_deg)),{convert(frc::Translation2d(14_m,cable_bump?1.22_m:4.4_m)),convert(frc::Translation2d(12_m,cable_bump?1.22_m:4.4_m))},convert(frc::Pose2d(12_m,cable_bump?3_m:2_m,0_deg)), conf);
	AddCommands(
		frc2::FunctionalCommand(
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
		),
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
		DriveForwardThenBalance(drive, 0.35)
	);
}