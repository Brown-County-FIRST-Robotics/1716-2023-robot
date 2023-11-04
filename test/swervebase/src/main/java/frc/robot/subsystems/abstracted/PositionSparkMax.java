package frc.robot.subsystems.abstracted;

import com.revrobotics.*;
import edu.wpi.first.networktables.GenericEntry;
import edu.wpi.first.networktables.NetworkTableValue;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;

public class PositionSparkMax implements PositionMotor {
  private final SparkMaxPIDController pid;
  private final SparkMaxAnalogSensor encoder;
  private final GenericEntry encpos;
  private final GenericEntry setspoint;
  CANSparkMax motor;

  public PositionSparkMax(int CANID, double KV, double p, double i, double d) {
    motor = new CANSparkMax(CANID, CANSparkMaxLowLevel.MotorType.kBrushless);
    motor.restoreFactoryDefaults();
    pid = motor.getPIDController();
    encoder = motor.getAnalog(SparkMaxAnalogSensor.Mode.kAbsolute);

    encoder.setPositionConversionFactor(1 / 3.3);
    encoder.setInverted(true);
    pid.setFeedbackDevice(encoder);

    pid.setP(p);
    pid.setI(i);
    pid.setD(d);
    pid.setFF(KV);
    pid.setOutputRange(-1, 1);
    pid.setSmartMotionMaxVelocity(300, 0);
    pid.setSmartMotionMinOutputVelocity(0, 0);
    pid.setSmartMotionMaxAccel(300, 0);
    pid.setSmartMotionAllowedClosedLoopError(0.005, 0);
    pid.setPositionPIDWrappingEnabled(true);
    pid.setPositionPIDWrappingMaxInput(1);
    pid.setPositionPIDWrappingMinInput(0);
    motor.setSmartCurrentLimit(20);
    motor.burnFlash();
    setspoint = Shuffleboard.getTab("Logging").add("FL Pos SP", 0.0).getEntry();
    encpos = Shuffleboard.getTab("Logging").add("FL Pos PV", 0.0).getEntry();
  }

  @Override
  public void setPos(double cmd_pos) {
    double pos=((cmd_pos%1.0)+1.0)%1.0;
    setspoint.set(NetworkTableValue.makeDouble(pos));
    pid.setReference(pos, CANSparkMax.ControlType.kSmartMotion);
  }

  @Override
  public double getPos() {
    encpos.set(NetworkTableValue.makeDouble(encoder.getPosition()));
    return encoder.getPosition();
  }
}
