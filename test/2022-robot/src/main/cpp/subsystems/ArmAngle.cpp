#include "Constants.h"
#include "subsystems/ArmAngle.h"

ArmAngle::ArmAngle() : 
angleMotor1{ArmAngleConstants::ANGLEMOTOR1ID}, 
angleMotor2{ArmAngleConstants::ANGLEMOTOR2ID}, 
angleMotors{angleMotor1, angleMotor2} {}

void ArmAngle::SetArm(double speed) {
	angleMotors.Set(speed);
}