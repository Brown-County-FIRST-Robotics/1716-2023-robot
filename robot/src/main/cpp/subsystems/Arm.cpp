#include "subsystems/Arm.h"
#include <frc/SmartDashboard/SmartDashboard.h>
#include <iostream>

Arm::Arm(frc::PneumaticHub& hubRef) : hub{hubRef}, elbowPid{elbow.GetPIDController()}, elbowEncoder{elbow.GetEncoder()},
	shoulderPid{Shoulder_P, 0, 0} {
	elbow.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
	shoulder.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);

	elbowEncoder.SetPosition(0);
	elbow.SetInverted(true);

	elbowPid.SetP(P);
    elbowPid.SetI(I);
    elbowPid.SetD(D);
    elbowPid.SetIZone(0);
    elbowPid.SetFF(0);
    elbowPid.SetOutputRange(-MAX_OUTPUT, MAX_OUTPUT);

	frc::SmartDashboard::PutNumber("P", P);
	frc::SmartDashboard::PutNumber("I", I);
	frc::SmartDashboard::PutNumber("D", D);
	frc::SmartDashboard::PutNumber("Max Output", MAX_OUTPUT);
	frc::SmartDashboard::PutNumber("Position", elbowPositionGoal);


	frc::SmartDashboard::PutNumber("shoulder P", Shoulder_P);


	shoulderPositionGoal = shoulderEncoder.Get();
	frc::SmartDashboard::PutNumber("Shoulder Pos", shoulderPositionGoal);
}

void Arm::Periodic() {
	if (clawTicks == 0)
		claw.Set(frc::DoubleSolenoid::Value::kOff);
	else if (clawTicks > -1)
		clawTicks--;

	double p = frc::SmartDashboard::GetNumber("P", 0);
	double i = frc::SmartDashboard::GetNumber("I", 0);
	double d = frc::SmartDashboard::GetNumber("D", 0);
	double max = frc::SmartDashboard::GetNumber("Max Output", 0);
	double pos = frc::SmartDashboard::GetNumber("Position", 0);

	if((p != P)) { elbowPid.SetP(p); P = p; }
    if((i != I)) { elbowPid.SetI(i); I = i; }
    if((d != D)) { elbowPid.SetD(d); D = d; }
    if((max != MAX_OUTPUT)) { elbowPid.SetOutputRange(-max, max); MAX_OUTPUT = max; }
    if((pos != elbowPositionGoal)) { elbowPid.SetReference(pos, rev::CANSparkMax::ControlType::kPosition); elbowPositionGoal = pos; }
	frc::SmartDashboard::PutNumber("Output", elbow.GetAppliedOutput());


	frc::SmartDashboard::PutNumber("Shoulder encoder", shoulderEncoder.Get());
	shoulderPositionGoal = frc::SmartDashboard::GetNumber("Shoulder Pos", shoulderPositionGoal);


	double shoulderp = frc::SmartDashboard::GetNumber("P", 0);
	if((shoulderp != Shoulder_P)) { shoulderPid.SetP(shoulderp); Shoulder_P = shoulderp; }


	if (shoulderActive) {
		double output = std::clamp(shoulderPid.Calculate(shoulderEncoder.Get(), shoulderPositionGoal), -ArmConst::SHOULDER_SPEED, ArmConst::SHOULDER_SPEED);
		if (output > 0 && shoulderEncoder.Get() > 78) //enforce limits
			// shoulder.Set(output);
			std::cout << output;
		else if (output < 0 && shoulderEncoder.Get() < 130)
			// shoulder.Set(output);
			std::cout << output;
		else
			// shoulder.Set(0);
			std::cout << 0;
	}
}

void Arm::SetShoulderGoal(double position) {
	// shoulderPositionGoal = position;
}

double Arm::GetShoulderGoal() {
	return shoulderPositionGoal;
}

double Arm::GetShoulderPosition() {
	return shoulderEncoder.Get();
}

void Arm::SetShoulderActive(bool activate) {
	shoulderActive = activate;
}

void Arm::SetElbowGoal(double position) {
	// elbowPid.SetReference(position, rev::CANSparkMax::ControlType::kPosition);
	// elbowPositionGoal = position;
}

double Arm::GetElbowGoal() {
	return elbowPositionGoal;
}

double Arm::GetElbowPosition() {
	return elbowEncoder.GetPosition();
}

void Arm::SetElbowActive(bool activateElbow) {
	elbowPid.SetReference(elbowEncoder.GetPosition(), rev::CANSparkMax::ControlType::kPosition); //TEST ENCODER UNITS
}

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
