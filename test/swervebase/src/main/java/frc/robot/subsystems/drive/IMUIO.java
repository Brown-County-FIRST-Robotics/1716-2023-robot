package frc.robot.subsystems.drive;

import edu.wpi.first.math.geometry.Rotation2d;
import org.littletonrobotics.junction.AutoLog;

public interface IMUIO {
  @AutoLog
  public static class IMUIOInputs {
    public Rotation2d yaw = Rotation2d.fromRotations(0);
    public Rotation2d roll = Rotation2d.fromRotations(0);
    public Rotation2d pitch = Rotation2d.fromRotations(0);

    public double xAccelMPS = 0.0;
    public double yAccelMPS = 0.0;
    public double zAccelMPS = 0.0;
    public double tempC = 0.0;
  }

  public default void updateInputs(IMUIOInputs inputs) {}
  ;
}
