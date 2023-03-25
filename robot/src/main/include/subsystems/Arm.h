#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>
#include <rev/CANSparkMax.h>
#include <frc/AnalogPotentiometer.h>
#include <frc/PneumaticHub.h>
#include <frc/controller/PIDController.h>

#include "Constants.h"

class Arm : public frc2::SubsystemBase {
public:
	Arm(frc::PneumaticHub& hubRef);

	void Periodic() override;

	void SetShoulderGoal(double position);
	double GetShoulderGoal();
	double GetShoulderPosition();
	void SetShoulderActive(bool activate);

	void SetElbowGoal(double position);
	double GetElbowGoal();
	double GetElbowPosition();
	void SetElbowActive(bool activate);

	void ToggleClaw();
	void SetClaw(frc::DoubleSolenoid::Value value);
	frc::DoubleSolenoid::Value GetClaw();

private:
	//shoulder
	rev::CANSparkMax shoulder{ArmConst::SHOULDER_ID, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
	frc::AnalogPotentiometer shoulderEncoder{0, -230, 206}; //set up for degrees
	frc2::PIDController shoulderPid{ArmConst::SHOULDER_P, 0, 0};
	double shoulderPositionGoal;
	double shoulderPidOutput;

	//elbow
	rev::CANSparkMax elbow{ArmConst::ELBOW_ID, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
	rev::SparkMaxPIDController elbowPid;
	rev::SparkMaxRelativeEncoder elbowEncoder;
	double elbowPositionGoal = 0;
	rev::SparkMaxLimitSwitch elbowOutLimit;
	rev::SparkMaxLimitSwitch elbowInLimit;

	//claw
	frc::PneumaticHub& hub;
	frc::DoubleSolenoid claw = hub.MakeDoubleSolenoid(ArmConst::CLAW_ID[0], ArmConst::CLAW_ID[1]);
	int clawTicks = -1;
	frc::DoubleSolenoid::Value clawPos = frc::DoubleSolenoid::Value::kReverse;


//TEMP CODE: ELBOW PID CONFIG
	double elbowP = 0.08;
	double elbowI = 0.0002;
	double elbowD = 2;
	double elbowMaxSpeed = 0.2;
//END TEMP CODE: ELBOW PID CONFIG
};
