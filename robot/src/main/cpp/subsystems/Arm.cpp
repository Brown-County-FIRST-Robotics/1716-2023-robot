#include "subsystems/Arm.h"
#include <math.h>

Arm::Arm() : 
	elbowEncoder{elbow.GetEncoder()}, 
	shoulderEncoder{shoulder.GetEncoder()}, 
{
	elbow.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
	shoulder.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
}

bool Arm::SetToAngle(double elbow_ang, double shoulder_ang){
	double elbow_pos=elbowEncoder.GetPosition();
	elbow.Set(ArmConst::ELBOW_P*(elbow_pos-elbow_ang));
	double shoulder_pos=shoulderEncoder.GetPosition();
	shoulder.Set(ArmConst::SHOULDER_P*(shoulder_pos-shoulder_ang));

	if(fabs(elbow_pos-elbow_ang)<ArmConst::ELBOW_THRESHOLD){
		return true;
	}elif(fabs(shoulder_pos-shoulder_ang)<ArmConst::SHOULDER_THRESHOLD){
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


