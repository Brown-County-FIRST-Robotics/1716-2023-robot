package frc.robot;

import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.math.kinematics.SwerveDriveKinematics;

public class Constants {
  public static final class IO {
    // TODO: add real values
    public static final int FL_STEER_ID = 0;
    public static final int FR_STEER_ID = 2;
    public static final int BL_STEER_ID = 1;
    public static final int BR_STEER_ID = 3;
    public static final int FL_DRIVE_ID = 0;
    public static final int FR_DRIVE_ID = 2;
    public static final int BL_DRIVE_ID = 1;
    public static final int BR_DRIVE_ID = 3;
  }

  public static final class Drivetrain {
    // TODO: add real values
    public static final double FL_STEER_P = 0;
    public static final double FL_STEER_I = 0;
    public static final double FL_STEER_D = 0;
    public static final double FL_STEER_FF = 1;

    public static final double FL_DRIVE_P = 0;
    public static final double FL_DRIVE_I = 0;
    public static final double FL_DRIVE_D = 0;
    public static final double FL_DRIVE_FF = 1;

    public static final double FR_STEER_P = 0;
    public static final double FR_STEER_I = 0;
    public static final double FR_STEER_D = 0;
    public static final double FR_STEER_FF = 1;
    public static final double FR_DRIVE_P = 0;
    public static final double FR_DRIVE_I = 0;
    public static final double FR_DRIVE_D = 0;
    public static final double FR_DRIVE_FF = 1;

    public static final double BL_STEER_P = 0;
    public static final double BL_STEER_I = 0;
    public static final double BL_STEER_D = 0;
    public static final double BL_STEER_FF = 1;
    public static final double BL_DRIVE_P = 0;
    public static final double BL_DRIVE_I = 0;
    public static final double BL_DRIVE_D = 0;
    public static final double BL_DRIVE_FF = 1;

    public static final double BR_STEER_P = 0;
    public static final double BR_STEER_I = 0;
    public static final double BR_STEER_D = 0;
    public static final double BR_STEER_FF = 1;
    public static final double BR_DRIVE_P = 0;
    public static final double BR_DRIVE_I = 0;
    public static final double BR_DRIVE_D = 0;
    public static final double BR_DRIVE_FF = 1;

    public static final double EFFECTIVE_WHEEL_DIAMETER = 1; // TODO: add real values
    public static final SwerveDriveKinematics KINEMATICS =
        new SwerveDriveKinematics(
            new Translation2d(-1, 1),
            new Translation2d(1, 1),
            new Translation2d(-1, -1),
            new Translation2d(1, -1));
  }

  public static final class Auto {
    public static final double X_P = 0;
    public static final double X_I = 0;
    public static final double X_D = 0;

    public static final double Y_P = 0;
    public static final double Y_I = 0;
    public static final double Y_D = 0;

    public static final double THETA_P = 0;
    public static final double THETA_I = 0;
    public static final double THETA_D = 0;

    public static final double MAX_ANGULAR_VELOCITY = 0;
    public static final double MAX_ANGULAR_ACCELERATION = 0;
    public static final double MAX_VELOCITY = 0;
    public static final double MAX_ACCELERATION = 0;
  }

  public static final Pose2d INIT_POSE = new Pose2d(0, 0, new Rotation2d(0));
}
