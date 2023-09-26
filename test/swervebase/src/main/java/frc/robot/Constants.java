package frc.robot;

import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.math.kinematics.SwerveDriveKinematics;

public class Constants {

  public class SwerveModuleConstants {
    public double steerKV;
    public double steerP;
    public double steerI;
    public double steerD;
    public double steerOffset;
    public double steerRotationsPerAngle;

    public double thrustKV;
    public double thrustP;
    public double thrustI;
    public double thrustD;

    public int steerID;
    public int thrustID;
    public int encoderID;

    public SwerveModuleConstants(
        double sk,
        double sp,
        double si,
        double sd,
        double so,
        double sr,
        double tk,
        double tp,
        double ti,
        double td,
        int sid,
        int tid,
        int eid) {
      steerKV = sk;
      steerP = sp;
      steerI = si;
      steerD = sd;
      steerOffset = so;
      steerRotationsPerAngle = sr;
      thrustKV = tk;
      thrustP = tp;
      thrustI = ti;
      thrustD = td;

      steerID = sid;
      thrustID = tid;
      encoderID = eid;
    }

    public double getRotationsFromAngle(double angRads) {
      return (angRads - steerOffset) * steerRotationsPerAngle;
    }

    public double getAngleFromRotations(double rotations) {
      return (rotations / steerRotationsPerAngle) + steerOffset;
    }
  }

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

  public final class Drivetrain {
    // TODO: add real values
    public final SwerveModuleConstants FL =
        new SwerveModuleConstants(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    public static final double EFFECTIVE_WHEEL_DIAMETER = 1; // TODO: add real values
    public final SwerveDriveKinematics KINEMATICS =
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
