package frc.robot.subsystems.arm;

import com.revrobotics.*;

public class ArmSparkMaxIO implements ArmIO {
  public static final double gearRatio = (12.75 * (64.0 / 14.0)) / (2* Math.PI);

  private final CANSparkMax motor;
  private final SparkMaxPIDController pid;
  private final SparkMaxLimitSwitch inSwitch;
  private final SparkMaxLimitSwitch outSwitch;
  private final RelativeEncoder enc;
  private double arbff=0.9;

  public double getAng() {
    return enc.getPosition() / gearRatio;
  }

  public ArmSparkMaxIO() {
    motor = new CANSparkMax(51, CANSparkMaxLowLevel.MotorType.kBrushless);
    pid = motor.getPIDController();
    enc = motor.getEncoder();
    inSwitch = motor.getReverseLimitSwitch(SparkMaxLimitSwitch.Type.kNormallyClosed);
    outSwitch = motor.getForwardLimitSwitch(SparkMaxLimitSwitch.Type.kNormallyClosed);
    motor.setIdleMode(CANSparkMax.IdleMode.kBrake);
    motor.setInverted(true);
    pid.setP(0);
    pid.setI(0);
    pid.setD(0);
    pid.setFF(0.0002);
    pid.setOutputRange(-1, 1);
    pid.setSmartMotionMaxVelocity(5000, 0);
    pid.setSmartMotionMaxAccel(2000, 0);
    enc.setPosition(0);
    pid.setSmartMotionMinOutputVelocity(0,0);
    pid.setSmartMotionAllowedClosedLoopError(1,0);
  }

  @Override
  public void updateInputs(ArmIOInputs inputs) {
    inputs.inSwitch = inSwitch.isPressed();
    inputs.outSwitch = outSwitch.isPressed();
    inputs.posRads = getAng();
    inputs.appliedVolts = motor.getAppliedOutput() * motor.getBusVoltage();
  }

  @Override
  public void setGoalPos(double pos) {
    pid.setReference(
        pos * gearRatio,
        CANSparkMax.ControlType.kSmartMotion,
        0,
        Math.sin(getAng())*arbff,
        SparkMaxPIDController.ArbFFUnits.kVoltage);
  }

  @Override
  public void setArbff(double arbff) {
    this.arbff = arbff;
  }

  @Override
  public void setEncPos(double pos) {
    enc.setPosition(pos * gearRatio);
  }
}
