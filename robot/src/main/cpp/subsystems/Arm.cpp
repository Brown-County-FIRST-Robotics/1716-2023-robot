#include "subsystems/Arm.h"
#include <frc/SmartDashboard/SmartDashboard.h>
#include <iostream>

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
	elbowPid.SetP(elbowP);
    elbowPid.SetI(elbowI);
    elbowPid.SetD(elbowD);
    elbowPid.SetIZone(0);
    elbowPid.SetFF(0);
    elbowPid.SetOutputRange(-elbowMaxSpeed, elbowMaxSpeed);


	shoulderPid.SetSetpoint(shoulderEncoder.Get());
	// shoulderPid.SetTolerance(ArmConst::SHOULDER_PID_TOLERANCE);


//TEMP CODE: PID SHUFFLEBOARD CONFIG
	// frc::SmartDashboard::PutNumber("elbowP", elbowP);
	// frc::SmartDashboard::PutNumber("elbowI", elbowI);
	// frc::SmartDashboard::PutNumber("elbowD", elbowD);
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
    // if((pos != elbowGoal)) { elbowPid.SetReference(pos, rev::CANSparkMax::ControlType::kPosition); elbowGoal = pos; }
	// frc::SmartDashboard::PutNumber("Elbow Output", elbow.GetAppliedOutput());

	// frc::SmartDashboard::PutNumber("Shoulder encoder", shoulderEncoder.Get());
	// shoulderPositionGoal = frc::SmartDashboard::GetNumber("Shoulder Pos", shoulderPositionGoal);
//END TEMP CODE: PID SHUFFLEBOARD CONFIG


	//shoulder PID control
	shoulderPidOutput = std::clamp(shoulderPid.Calculate(shoulderEncoder.Get()), -ArmConst::SHOULDER_MAX_SPEED, ArmConst::SHOULDER_MAX_SPEED); 
		//cache pid output with a limit for safety for use in if statements

	if (shoulderPidOutput < 0 && shoulderEncoder.Get() > ArmConst::SHOULDER_EXTREME[0])// && !shoulderPid.AtSetpoint()) //enforce limits
		shoulder.Set(shoulderPidOutput);
	else if (shoulderPidOutput > 0 && shoulderEncoder.Get() < ArmConst::SHOULDER_EXTREME[1])// && !shoulderPid.AtSetpoint())
		shoulder.Set(shoulderPidOutput);
	else
		shoulder.Set(0);


//TEMP CODE: LIMIT SWITH TESTING
	// frc::SmartDashboard::PutBoolean("Elbow out limit", elbowInLimit.Get());
	// frc::SmartDashboard::PutBoolean("Elbow in limit", elbowOutLimit.Get());
//END TEMP CODE: LIMIT SWITCH TESTING

	//log goals and positions
	frc::SmartDashboard::PutNumber("Shoulder Goal", shoulderPid.GetSetpoint());
	frc::SmartDashboard::PutNumber("Elbow Goal", elbowGoal);
	frc::SmartDashboard::PutNumber("Shoulder Position", shoulderEncoder.Get());
	frc::SmartDashboard::PutNumber("Elbow Position", elbowEncoder.GetPosition());


	//reset elbow encoder on limit switch
	if (elbowOutLimit.Get() && !touchingLimit) {
		elbowEncoder.SetPosition(0);
		elbowPid.SetReference(0, rev::CANSparkMax::ControlType::kPosition);
		elbowGoal = 0;
		touchingLimit = true;
	}
	else if (!elbowOutLimit.Get())
		touchingLimit = false;
}

//shoulder methods
void Arm::SetShoulderGoal(double position) {
	shoulderPid.SetSetpoint(position);
}

void Arm::AddToShoulderGoal(double value) {
	shoulderPid.SetSetpoint(shoulderPid.GetSetpoint() + value);
}

double Arm::GetShoulderGoal() {
	return shoulderPid.GetSetpoint();
}

double Arm::GetShoulderPosition() {
	return shoulderEncoder.Get();
}

void Arm::StopShoulder() {
	shoulderPid.SetSetpoint(shoulderEncoder.Get());
}

//elbow methods
void Arm::SetElbowGoal(double position) {
	elbowPid.SetReference(position, rev::CANSparkMax::ControlType::kPosition);
	elbowGoal = position;
}

void Arm::AddToElbowGoal(double value) {
	elbowPid.SetReference(elbowGoal + value, rev::CANSparkMax::ControlType::kPosition);
	elbowGoal += value;
}

double Arm::GetElbowGoal() {
	return elbowGoal;
}

double Arm::GetElbowPosition() {
	return elbowEncoder.GetPosition();
}

void Arm::StopElbow() {
	elbowPid.SetReference(elbowEncoder.GetPosition(), rev::CANSparkMax::ControlType::kPosition);
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
