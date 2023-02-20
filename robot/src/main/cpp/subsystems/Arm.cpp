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


