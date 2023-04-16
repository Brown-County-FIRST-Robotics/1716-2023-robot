#define _USE_MATH_DEFINES

#include "subsystems/Arm.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <iostream>
#include <cmath>

Arm::Arm(frc::PneumaticHub& hubRef) : hub{hubRef}, elbowPid{elbow.GetPIDController()}, elbowEncoder{elbow.GetEncoder()},
	elbowInLimit{elbow.GetReverseLimitSwitch(rev::SparkMaxLimitSwitch::Type::kNormallyClosed)}, 
	elbowOutLimit{elbow.GetForwardLimitSwitch(rev::SparkMaxLimitSwitch::Type::kNormallyClosed)} 
	{
	//configure motors and encoders
	shoulder.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
	shoulder.SetInverted(true);
	elbow.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
	elbow.SetInverted(true);
	elbowEncoder.SetPosition(0);

	//configure PID
	elbowPid.SetP(ArmConst::ELBOW_P);
    elbowPid.SetI(ArmConst::ELBOW_I);
    elbowPid.SetD(ArmConst::ELBOW_D);
    elbowPid.SetIZone(ArmConst::ELBOW_IZONE);
    elbowPid.SetFF(ArmConst::ELBOW_FEED_FORWARD);
    elbowPid.SetOutputRange(-ArmConst::ELBOW_MAX_OUTPUT, ArmConst::ELBOW_MAX_OUTPUT);

	shoulderPid.SetSetpoint(GetShoulderPosition());
	shoulderPid.SetTolerance(ArmConst::SHOULDER_PID_TOLERANCE);

	//configure smartmotion
	elbowPid.SetSmartMotionAllowedClosedLoopError(ArmConst::CLOSED_LOOP_ERROR);
	elbowPid.SetSmartMotionMaxVelocity(ArmConst::MAX_VELOCITY);
	elbowPid.SetSmartMotionMinOutputVelocity(ArmConst::MIN_VELOCITY);
	elbowPid.SetSmartMotionMaxAccel(ArmConst::MAX_ACCEL);

//TEMP CODE: PID SHUFFLEBOARD CONFIG
	// frc::SmartDashboard::PutNumber("elbowP", elbowP);
	 //frc::SmartDashboard::PutNumber("elbowI", elbowI);
	 //frc::SmartDashboard::PutNumber("elbowD", elbowD);
	// frc::SmartDashboard::PutNumber("Max Output", elbowMaxSpeed);
	// frc::SmartDashboard::PutNumber("Position", elbowGoal);

	// frc::SmartDashboard::PutNumber("shoulder elbowP", Shoulder_P);
	// frc::SmartDashboard::PutNumber("Shoulder Pos", shoulderPositionGoal);
//END TEMP CODE: PID SHUFFLEBOARD CONFIG
}

void Arm::Periodic() {
	//solenoid timeout
	if (clawTicks == 0)
		claw.Set(frc::DoubleSolenoid::Value::kOff);
	else if (clawTicks > -1)
		clawTicks--;


//TEMP CODE: PID SHUFFLEBOARD CONFIG
	// double p = frc::SmartDashboard::GetNumber("elbowP", 0);
	// double i = frc::SmartDashboard::GetNumber("elbowI", 0);
	// double d = frc::SmartDashboard::GetNumber("elbowD", 0);
	// double max = frc::SmartDashboard::GetNumber("Max Output", 0);
	// double pos = frc::SmartDashboard::GetNumber("Position", 0);

	// if((p != elbowP)) { elbowPid.SetP(p); elbowP = p; }
    // if((i != elbowI)) { elbowPid.SetI(i); elbowI = i; }
    // if((d != elbowD)) { elbowPid.SetD(d); elbowD = d; }
    // if((max != elbowMaxSpeed)) { elbowPid.SetOutputRange(-max, max); elbowMaxSpeed = max; }
    // if((pos != elbowGoal)) { elbowPid.SetReference(pos, rev::CANSparkMax::ControlType::kSmartMotion, 0, sin(GetElbowAngleToGround() * (M_PI / 180)) * ArmConst::ELBOW_ARBITRARY_FEED_FORWARD); elbowGoal = pos; }
	// frc::SmartDashboard::PutNumber("Elbow Output", elbow.GetAppliedOutput());

	// frc::SmartDashboard::PutNumber("Shoulder encoder", GetShoulderPosition());
	// shoulderPositionGoal = frc::SmartDashboard::GetNumber("Shoulder Pos", shoulderPositionGoal);
//END TEMP CODE: PID SHUFFLEBOARD CONFIG


	//shoulder PID control
	shoulderPidOutput = std::clamp(shoulderPid.Calculate(GetShoulderPosition()), -ArmConst::SHOULDER_MAX_SPEED, ArmConst::SHOULDER_MAX_SPEED); 
		//cache pid output with a limit for safety for use in if statements
		//std::cout << "encoder pos:" << GetShoulderPosition() << std::endl;
	//std::cout << "shoulderPID" << shoulderPidOutput	 << std::endl;
	if (shoulderPidOutput < 0 && GetShoulderPosition() > ArmConst::SHOULDER_EXTREME[0]){// && !shoulderPid.AtSetpoint()) //enforce limits
		//std::cout << "at extreme 1:" << shoulderPidOutput << std::endl;
		shoulder.Set(shoulderPidOutput);
	}
	else if (shoulderPidOutput > 0 && GetShoulderPosition() < ArmConst::SHOULDER_EXTREME[1]){// && !shoulderPid.AtSetpoint())
		//std::cout << "at extreme 2:" << shoulderPidOutput << std::endl;
		shoulder.Set(shoulderPidOutput);
	}
	else{
		shoulder.Set(0);
		//std::cout << "None\n";
	}


//TEMP CODE: LIMIT SWITH TESTING
	// frc::SmartDashboard::PutBoolean("Elbow out limit", elbowInLimit.Get());
	// frc::SmartDashboard::PutBoolean("Elbow in limit", elbowOutLimit.Get());
//END TEMP CODE: LIMIT SWITCH TESTING

	//log goals and positions
	//frc::SmartDashboard::PutNumber("Shoulder Goal", shoulderPid.GetSetpoint());
	//frc::SmartDashboard::PutNumber("Elbow Goal", elbowGoal);
	//frc::SmartDashboard::PutNumber("Shoulder Position", GetShoulderPosition());
	//frc::SmartDashboard::PutNumber("Elbow Position", elbowEncoder.GetPosition());
	//std::cout << "Shoulder Goal: " << shoulderPid.GetSetpoint() << "\n";
	//std::cout << "Shoulder Position: " << GetShoulderPosition() << "\n";
	//std::cout << "Elbow Goal: " << elbowGoal << "\n";
	std::cout << "Version: 12\n";
	//std::cout << elbowPid.GetFF() << "\n";


	//reset elbow encoder on limit switch
	if (elbowOutLimit.Get() && !touchingOutLimit) {
		elbowEncoder.SetPosition(0);
		elbowPid.SetReference(0, rev::CANSparkMax::ControlType::kSmartMotion, 0, sin(GetElbowAngleToGround() * (M_PI / 180)) * ArmConst::ELBOW_ARBITRARY_FEED_FORWARD);
		elbowGoal = 0;
		touchingOutLimit = true;
	}
	else if (!elbowOutLimit.Get())
		touchingOutLimit = false;

	std::cout << "Elbo raw:" << elbowEncoder.GetPosition() << '\n';
	if (elbowInLimit.Get() && !touchingInLimit) {
		elbowGoal = ArmConst::ELBOW_IN_LIMIT_POS * ArmConst::ELBOW_ROTAIONS_TO_ANGLE_RATIO;
		elbowEncoder.SetPosition(elbowGoal);
		elbowPid.SetReference(elbowGoal, rev::CANSparkMax::ControlType::kSmartMotion, 0, sin(GetElbowAngleToGround() * (M_PI / 180)) * ArmConst::ELBOW_ARBITRARY_FEED_FORWARD);
		touchingInLimit = true;
	}
	else if (!elbowInLimit.Get())
		touchingInLimit = false;

	elbowPid.SetReference(elbowGoal, rev::CANSparkMax::ControlType::kSmartMotion, 0, sin(GetElbowAngleToGround() * (M_PI / 180)) * ArmConst::ELBOW_ARBITRARY_FEED_FORWARD); //update feedforward

	if (isDoingDrivePreset && shoulderPid.AtSetpoint() && !drivePresetShoulderIsFinished) {
		drivePresetShoulderIsFinished = true;
		SetElbowGoal(ArmHeightConst::DRIVE[1]);
	}
	else if (drivePresetShoulderIsFinished && fabs(GetElbowGoal() - GetElbowPosition()) < 2) {
		isDoingDrivePreset = false;
	}
}

//shoulder methods
void Arm::SetShoulderGoal(double position) {
	shoulderGoal = position;
	shoulderPid.SetSetpoint(position);
}

void Arm::AddToShoulderGoal(double value) {
	//act kind of like velocity mode.  The human is commanding a change from where they see the arm currently is
	//this works better then just adding to goal because it can't get really big thithout the human knowing
	//while the arm is slow and then keep moving when the human lets go.
	shoulderGoal = GetShoulderPosition() + value; // Add the joystick value onto the encoder value
	shoulderPid.SetSetpoint(shoulderGoal);
}

double Arm::GetShoulderGoal() {
	return shoulderGoal;
}

double Arm::GetShoulderPosition() {
	return ((0.419 - shoulderEncoder.GetAbsolutePosition()) * 360) + 90;
}

void Arm::StopShoulder() {
	SetShoulderGoal(GetShoulderPosition());
}

//elbow methods
void Arm::SetElbowGoal(double position) {
	elbowPid.SetReference(position, rev::CANSparkMax::ControlType::kSmartMotion, 0, sin(GetElbowAngleToGround() * (M_PI / 180)) * ArmConst::ELBOW_ARBITRARY_FEED_FORWARD);
	elbowGoal = position;
}

void Arm::AddToElbowGoal(double value) {

	if (touchingOutLimit && value > 0)
		value = 0;
	if (touchingInLimit && value < ArmConst::ELBOW_IN_LIMIT_POS)
		value = ArmConst::ELBOW_IN_LIMIT_POS;
	//act kind of like velocity mode.  The human is commanding a change from where they see the arm currently is
	//this works better then just adding to goal because it can't get really big thithout the human knowing
	//while the arm is slow and then keep moving when the human lets go.
	elbowGoal = elbowEncoder.GetPosition() + value;
	elbowPid.SetReference(elbowGoal, rev::CANSparkMax::ControlType::kSmartMotion, 0, sin(GetElbowAngleToGround() * (M_PI / 180)) * ArmConst::ELBOW_ARBITRARY_FEED_FORWARD);
}

double Arm::GetElbowGoal() {
	return elbowGoal;
}

double Arm::GetElbowPosition() {
	return elbowEncoder.GetPosition();
}

void Arm::StopElbow() {
	SetElbowGoal(elbowEncoder.GetPosition());
}

//claw methods
void Arm::ToggleClaw() {
	if (clawPos == frc::DoubleSolenoid::Value::kReverse) { //if reverse, set to forward
		claw.Set(frc::DoubleSolenoid::Value::kForward);
		clawPos = frc::DoubleSolenoid::Value::kForward;
	}
	else { //if not reverse, set to reverse
		claw.Set(frc::DoubleSolenoid::Value::kReverse);
		clawPos = frc::DoubleSolenoid::Value::kReverse;
	}
	
	clawTicks = SolenoidConst::WAIT_TICKS;
}

void Arm::SetClaw(frc::DoubleSolenoid::Value value) {
	claw.Set(value);
	clawPos = value;
	
	clawTicks = SolenoidConst::WAIT_TICKS;
}

frc::DoubleSolenoid::Value Arm::GetClaw() {
	return clawPos;
}

void Arm::SetStowing(bool stowing) {
	if (stowing) {
		elbowPid.SetOutputRange(-ArmConst::ELBOW_MAX_OUTPUT * .5, ArmConst::ELBOW_MAX_OUTPUT * .5);
	}
	else {
		elbowPid.SetOutputRange(-ArmConst::ELBOW_MAX_OUTPUT, ArmConst::ELBOW_MAX_OUTPUT);
	}
}

double Arm::GetElbowAngleToGround() {
	double angle = ((elbowEncoder.GetPosition() / ArmConst::ELBOW_ROTAIONS_TO_ANGLE_RATIO) + 301.0) - (GetShoulderPosition() - 90.0);
		//arm straight down is 0 and arm sticking out is 90
	std::cout << "Elbow angle from ground: " << angle << "\n";
	return angle;
}

void Arm::InitiateDrivePreset() {
	isDoingDrivePreset = true;
	drivePresetShoulderIsFinished = false;
	SetShoulderGoal(ArmHeightConst::DRIVE[0]);
}
