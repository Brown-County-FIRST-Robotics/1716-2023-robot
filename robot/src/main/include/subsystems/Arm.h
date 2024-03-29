#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/DoubleSolenoid.h>
#include <rev/CANSparkMax.h>
#include <frc/AnalogPotentiometer.h>
#include <frc/PneumaticHub.h>
#include <frc/controller/PIDController.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/DutyCycleEncoder.h>

#include "Constants.h"

class Arm : public frc2::SubsystemBase {
public:
	Arm(frc::PneumaticHub& hubRef);

	void Periodic() override;

	void SetShoulderGoal(double position);
	void AddToShoulderGoal(double value);
	double GetShoulderGoal();
	double GetShoulderPosition();
	void StopShoulder(); //set the goal to the current position

	void SetElbowGoal(double position);
	void AddToElbowGoal(double value);
	double GetElbowGoal();
	double GetElbowPosition();
	void StopElbow(); //set the goal to the current position

	void ToggleClaw();
	void SetClaw(frc::DoubleSolenoid::Value value);
	frc::DoubleSolenoid::Value GetClaw();

	void SetStowing(bool stowing);

	void InitiateDrivePreset();

private:
	//shoulder
	rev::CANSparkMax shoulder{ArmConst::SHOULDER_ID, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
	frc::DutyCycleEncoder shoulderEncoder{0};
	frc2::PIDController shoulderPid{ArmConst::SHOULDER_P, 0, 0};
	double shoulderPidOutput;
	double shoulderGoal = 88;

	//elbow
	rev::CANSparkMax elbow{ArmConst::ELBOW_ID, rev::CANSparkMaxLowLevel::MotorType::kBrushless};
	rev::SparkMaxPIDController elbowPid;
	rev::SparkMaxRelativeEncoder elbowEncoder;
	double elbowGoal = 0;
	rev::SparkMaxLimitSwitch elbowInLimit;
	rev::SparkMaxLimitSwitch elbowOutLimit;
	bool touchingInLimit = false;
	bool touchingOutLimit = false;
	double GetElbowAngleToGround();

	//claw
	frc::PneumaticHub& hub;
	frc::DoubleSolenoid claw = hub.MakeDoubleSolenoid(ArmConst::CLAW_ID[0], ArmConst::CLAW_ID[1]);
	int clawTicks = -1;
	frc::DoubleSolenoid::Value clawPos = frc::DoubleSolenoid::Value::kReverse;

	//drive preset
	bool drivePresetShoulderIsFinished = false;
	bool isDoingDrivePreset = false;

//TEMP CODE: ELBOW PID CONFIG
//	double elbowP = 0.08;
//	double elbowI = 0.0002;
//	double elbowD = 2;
//	double elbowMaxSpeed = 0.2;
//END TEMP CODE: ELBOW PID CONFIG
};
