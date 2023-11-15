package frc.robot.subsystems.arm;

import org.littletonrobotics.junction.AutoLog;

public interface ArmIO {
  @AutoLog
  public static class ArmIOInputs {
    public double posRads = 0.0;
    public double appliedVolts = 0.0;
    public boolean inSwitch = false;
    public boolean outSwitch = false;
  }

  public default void updateInputs(ArmIOInputs inputs) {}

  public default void setGoalPos(double pos) {}

  public default void setEncPos(double pos) {}
  public default void setArbff(double arbff) {}

}
