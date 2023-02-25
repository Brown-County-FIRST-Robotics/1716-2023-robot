#define _USE_MATH_DEFINES

#include "subsystems/Arm.h"
#include <math.h>

Arm::Arm() : 
	shoulderEncoder{shoulder.GetEncoder()}, 
	elbowEncoder{elbow.GetEncoder()}
{
	shoulder.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
	elbow.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
}

bool Arm::SetToPosition(double xGoal, double yGoal) {
	x = xGoal;
	y = yGoal;
	double shoulderAngleGoal = (acos(((ArmConst::UPPER_ARM_LENGTH*ArmConst::UPPER_ARM_LENGTH) 
		+ (xGoal*xGoal) + (yGoal*yGoal) - (ArmConst::FOREARM_LENGTH*ArmConst::FOREARM_LENGTH))
		/ ((2 * ArmConst::UPPER_ARM_LENGTH) * sqrt((xGoal*xGoal) + (yGoal*yGoal))))
		+ atan2(yGoal, xGoal))
		* (180 / M_PI);
	double elbowAngleGoal = (acos(((ArmConst::UPPER_ARM_LENGTH*ArmConst::UPPER_ARM_LENGTH) - (xGoal*xGoal) + (ArmConst::FOREARM_LENGTH*ArmConst::FOREARM_LENGTH) - (yGoal*yGoal))
		/ (2 * ArmConst::UPPER_ARM_LENGTH * ArmConst::FOREARM_LENGTH)))
		* (180 / M_PI);

	double shoulderPos = GetEncoder(ArmConst::ID[0]);
	double elbowPos = GetEncoder(ArmConst::ID[1]);

	shoulder.Set(ArmConst::PROPORTIONAL[0] * (shoulderPos - shoulderAngleGoal));
	elbow.Set(ArmConst::PROPORTIONAL[1] * (elbowPos - elbowAngleGoal));

	if ((fabs(shoulderPos - shoulderAngleGoal) < ArmConst::THRESHOLD) && (fabs(elbowPos - elbowAngleGoal) < ArmConst::THRESHOLD))
		return true;
	else
		return false;
}

double Arm::GetEncoder(int motorID) {
	if (motorID == ArmConst::ID[0]) {
		return shoulderEncoder.GetPosition() / 42.0;
	}
	else if (motorID == ArmConst::ID[1]) {
		return elbowEncoder.GetPosition() / 42.0;
	}
	else {
		return 0;
	}
}

bool Arm::Zero(){
	if(!shoulderSwitch.Get())
		shoulder.Set(ArmConst::ZEROING_SPEED);
	else
		shoulder.Set(0);

	if(!elbowSwitch.Get())
		elbow.Set(ArmConst::ZEROING_SPEED);
	else
		elbow.Set(0);

	if(shoulderSwitch.Get() && elbowSwitch.Get()){
		elbowEncoder.SetPosition(0);
		shoulderEncoder.SetPosition(0);
		x = 0;
		y = 0;
		return true;
	}
	else
		return false;
}

double Arm::getX() {
	return x;
}

double Arm::getY() {
	return y;
}

void Arm::SetMotorsZero() {
	shoulder.Set(0);
	elbow.Set(0);
}
