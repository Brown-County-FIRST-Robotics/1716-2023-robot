package frc.robot;

import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.math.kinematics.MecanumDriveKinematics;

public class Constants {
  public static final class IO {
    // TODO: add real values
    public static final int FL_ID = 1;
    public static final int FR_ID = 2;
    public static final int BL_ID = 3;
    public static final int BR_ID = 4;

    public static final int[] DRIVETRAIN_SOLENOID_ID = {1, 1};
  }

  public static final class DRIVETRAIN {
    public static final MecanumDriveKinematics KINEMATICS =
        new MecanumDriveKinematics(
            new Translation2d(0, 0),
            new Translation2d(0, 0),
            new Translation2d(0, 0),
            new Translation2d(0, 0));
    public static final double EFFECTIVE_WHEEL_DIAMETER = 1;
    public static final Pose2d INIT_POSE = new Pose2d(1, 1, new Rotation2d(1));
    public static final int SOLENOID_WAIT_TICKS = 0;
  }
}
