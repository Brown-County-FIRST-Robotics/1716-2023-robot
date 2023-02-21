#include "subsystems/Arm.h"
#include <math.h>

Arm::Arm() : 
	shoulderEncoder{shoulder.GetEncoder()}, 
	elbowEncoder{elbow.GetEncoder()}
{
	shoulder.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
	elbow.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
}

bool Arm::SetToAngle(double shoulderAngle, double elbowAngle){
	double shoulderPos = shoulderEncoder.GetPosition();
	shoulder.Set(ArmConst::PROPORTIONAL[0] * (shoulderPos - shoulderAngle));

	double elbowPos=elbowEncoder.GetPosition();
	elbow.Set(ArmConst::PROPORTIONAL[1] * (elbowPos - elbowAngle));

	if(fabs(elbowPos-elbow_ang)<ArmConst::ELBOW_THRESHOLD){
		return true;
	}elif(fabs(shoulderPos-shoulder_ang)<ArmConst::SHOULDER_THRESHOLD){
		return true;
	}else{
		return false;
	}
}


bool Arm::ZeroArm(){
	if(!elbowSwitch.Get())
		elbow.Set(ArmConst::ELBOW_HOMING_SPEED);
	else
		elbow.Set(0);
	if(!shoulderSwitch.Get())
		shoulder.Set(ArmConst::SHOULDER_HOMING_SPEED);
	else
		shoulder.Set(0);
	if(shoulderSwitch.Get()&&elbowSwitch.Get()){
		elbowEncoder.SetPosition(0);
		shoulderEncoder.SetPosition(0);
	}
	return shoulderSwitch.Get() && elbowSwitch.Get();
}


