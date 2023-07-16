#include "commands/PlacePiece.h"


PlacePiece::PlacePiece(Drivetrain* drive, Arm* arm, LED* led, int height, int col){
    AddCommands(
        frc2::InstantCommand([drive, led, arm, height, col] {
            drive->SetSolenoid(DrivetrainConst::MECH_MODE);
            led->SetDrivetrainMode(drive->GetSolenoid());
            led->AddOscilateColor({0,0,255});
            // If there was an award for jankiest code, the next 2 lines would win
            //arm->SetElbowGoal(ArmHeightConst::HIGH[1]);
            ///arm->SetShoulderGoal(ArmHeightConst::HIGH[0]);
            },
            {drive, led, arm}
        ), 
        DriveToPoint(drive, AutoConst::POSES[col]),
        frc2::InstantCommand([drive, led, arm] {
            led->RemoveOscilateColor({0,0,255});
            //arm->SetClaw(ArmConst::CLAW_OPEN);
            //arm->InitiateDrivePreset();
            },
            {drive, led, arm}
        )
    );
}