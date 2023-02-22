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

	double elbowPos = elbowEncoder.GetPosition();
	elbow.Set(ArmConst::PROPORTIONAL[1] * (elbowPos - elbowAngle));

	if ((fabs(shoulderPos - shoulderAngle) < ArmConst::THRESHOLD) && (fabs(elbowPos - elbowAngle) < ArmConst::THRESHOLD))
		return true;
	else
		return false;
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
		return true;
	}
	else
		return false;
}
