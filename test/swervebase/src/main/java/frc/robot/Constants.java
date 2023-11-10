package frc.robot;

import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.math.kinematics.SwerveDriveKinematics;

public class Constants {

  public static final class IO {
    public static final int CONTROLLER1 = 0;
  }

  public static final class Drivetrain {
    public static final double THRUST_DISTANCE_PER_TICK = .0254 * 4.0 * Math.PI / (2048.0 * 6.75);
    public static final double THRUST_KV = 1023.0 * 600.0 / (6380.0 * 2048.0);
    public static final double STEER_KV = 1.0 / 300.0;
    public static final SwerveModuleConstants FL =
        new SwerveModuleConstants(
            STEER_KV, 0, 0, 0, 0.262, THRUST_KV, 0, 0, 0, THRUST_DISTANCE_PER_TICK, 10, 20);
    public static final SwerveModuleConstants FR =
        new SwerveModuleConstants(
            STEER_KV, 0, 0, 0, 0.817, THRUST_KV, 0, 0, 0, THRUST_DISTANCE_PER_TICK, 11, 21);
    public static final SwerveModuleConstants BL =
        new SwerveModuleConstants(
            STEER_KV, 0, 0, 0, -0.050, THRUST_KV, 0, 0, 0, THRUST_DISTANCE_PER_TICK, 12, 22);
    public static final SwerveModuleConstants BR =
        new SwerveModuleConstants(
            STEER_KV, 0, 0, 0, -0.250, THRUST_KV, 0, 0, 0, THRUST_DISTANCE_PER_TICK, 13, 23);
    public static final double MAX_WHEEL_SPEED = 20; // TODO: add real value
    public static final double D = 21.125 * 0.0254; // TODO: Rename this
    public static final SwerveDriveKinematics KINEMATICS =
        new SwerveDriveKinematics(
            new Translation2d(D / 2, D / 2),
            new Translation2d(D / 2, -D / 2),
            new Translation2d(-D / 2, D / 2),
            new Translation2d(-D / 2, -D / 2));
  }

  public static final class Driver {
    public static final double MAX_X_SPEED = 2;
    public static final double MAX_Y_SPEED = 2;
    public static final double MAX_THETA_SPEED = 6;
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

    public static final double MAX_ANGULAR_VELOCITY = 1;
    public static final double MAX_ANGULAR_ACCELERATION = 1;
    public static final double MAX_VELOCITY = 3;
    public static final double MAX_ACCELERATION = 1;
  }

  public static final Pose2d INIT_POSE = new Pose2d(0, 0, Rotation2d.fromDegrees(180));
  public static final Pose2d RED_INIT_POSE = new Pose2d(0, 0, Rotation2d.fromDegrees(180));
  public static final Pose2d BLUE_INIT_POSE = new Pose2d(0, 0, Rotation2d.fromDegrees(180));
}
