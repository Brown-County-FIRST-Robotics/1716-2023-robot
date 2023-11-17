package frc.robot.subsystems.drive;

import edu.wpi.first.math.controller.PIDController;
import edu.wpi.first.math.controller.ProfiledPIDController;
import edu.wpi.first.math.estimator.SwerveDrivePoseEstimator;
import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.math.kinematics.ChassisSpeeds;
import edu.wpi.first.math.kinematics.SwerveDriveKinematics;
import edu.wpi.first.math.kinematics.SwerveModulePosition;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import edu.wpi.first.math.trajectory.Trajectory;
import edu.wpi.first.math.trajectory.TrajectoryConfig;
import edu.wpi.first.math.trajectory.TrajectoryGenerator;
import edu.wpi.first.math.trajectory.TrapezoidProfile;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.smartdashboard.Field2d;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.SubsystemBase;
import edu.wpi.first.wpilibj2.command.SwerveControllerCommand;
import frc.robot.Constants;
import frc.robot.utils.SuppliedCommand;
import java.util.List;
import org.littletonrobotics.junction.Logger;

public class Drivetrain extends SubsystemBase {
  ModuleIO fl;
  ModuleIO fr;
  ModuleIO bl;
  ModuleIO br;
  ModuleIOInputsAutoLogged flInputs = new ModuleIOInputsAutoLogged();
  ModuleIOInputsAutoLogged frInputs = new ModuleIOInputsAutoLogged();
  ModuleIOInputsAutoLogged blInputs = new ModuleIOInputsAutoLogged();
  ModuleIOInputsAutoLogged brInputs = new ModuleIOInputsAutoLogged();

  IMUIO imu=new IMUIONavx();
  IMUIOInputsAutoLogged imuInputs = new IMUIOInputsAutoLogged();

  SwerveDrivePoseEstimator poseEstimator;
  Field2d field;

  public SwerveModulePosition[] getPositions() {
    return new SwerveModulePosition[] {
      new SwerveModulePosition(flInputs.thrustPos, Rotation2d.fromRotations(flInputs.steerPos)),
      new SwerveModulePosition(frInputs.thrustPos, Rotation2d.fromRotations(frInputs.steerPos)),
      new SwerveModulePosition(blInputs.thrustPos, Rotation2d.fromRotations(blInputs.steerPos)),
      new SwerveModulePosition(brInputs.thrustPos, Rotation2d.fromRotations(brInputs.steerPos))
    };
  }

  public Drivetrain(ModuleIO fl, ModuleIO fr, ModuleIO bl, ModuleIO br) {
    this.fl = fl;
    this.fr = fr;
    this.bl = bl;
    this.br = br;
    field = new Field2d();
    fl.updateInputs(flInputs);
    fr.updateInputs(frInputs);
    bl.updateInputs(blInputs);
    br.updateInputs(brInputs);
    poseEstimator =
        new SwerveDrivePoseEstimator(
            Constants.Drivetrain.KINEMATICS,
            getNavxRotation(),
            getPositions(),
            Constants.INIT_POSE);
    Shuffleboard.getTab("Debug").add("Pos sort of", field);
    setPos(new Pose2d(getPose().getTranslation(), new Rotation2d(0)));
  }

  @Override
  public void periodic() {
    imu.updateInputs(imuInputs);
    Logger.getInstance().processInputs("Drive/Gyro", imuInputs);
    fl.updateInputs(flInputs);
    fr.updateInputs(frInputs);
    bl.updateInputs(blInputs);
    br.updateInputs(brInputs);
    Logger.getInstance().processInputs("Drive/FL", flInputs);
    Logger.getInstance().processInputs("Drive/FR", frInputs);
    Logger.getInstance().processInputs("Drive/BL", blInputs);
    Logger.getInstance().processInputs("Drive/BR", brInputs);

    poseEstimator.update(getNavxRotation(), getPositions());
    Logger.getInstance().recordOutput("Drive/Pose", getPose());
    field.setRobotPose(getPose());
  }

  public Pose2d getPose() {
    return poseEstimator.getEstimatedPosition();
  }

  public Rotation2d getNavxRotation() {
    return Rotation2d.fromDegrees(imuInputs.yaw);
  }

  // Field-oriented drive (units are meters/second)
  public void drive(double x, double y, double theta, boolean foc) {
    ChassisSpeeds sp;
    if (foc) {
      sp = ChassisSpeeds.fromFieldRelativeSpeeds(-x, -y, -theta, getPose().getRotation().unaryMinus());
    } else {
      sp = new ChassisSpeeds(-x, -y, -theta);
    }
    SwerveModuleState[] states = Constants.Drivetrain.KINEMATICS.toSwerveModuleStates(sp);
    setModuleStates(states);
  }

  public void setModuleStates(SwerveModuleState[] states) {
    SwerveDriveKinematics.desaturateWheelSpeeds(states, Constants.Drivetrain.MAX_WHEEL_SPEED);
    Logger.getInstance().recordOutput("Drive/CmdStates", states);
    fl.setCmdState(states[0]);
    fr.setCmdState(states[1]);
    bl.setCmdState(states[2]);
    br.setCmdState(states[3]);
  }

  public Command makeTrajectoryCommand(Trajectory trajectory) {
    return new SwerveControllerCommand(
        trajectory,
        this::getPose,
        Constants.Drivetrain.KINEMATICS,
        new PIDController(0, 0, 0),
        new PIDController(0, 0, 0),
        new ProfiledPIDController(
            0,
            0,
            0,
            new TrapezoidProfile.Constraints(
                Constants.Auto.MAX_ANGULAR_VELOCITY, Constants.Auto.MAX_ANGULAR_ACCELERATION)),
        this::setModuleStates,
        this);
  }

  public Command makePositionCommand(Pose2d dest) {
    return new SuppliedCommand(
        () -> {
          TrajectoryConfig conf =
              new TrajectoryConfig(Constants.Auto.MAX_VELOCITY, Constants.Auto.MAX_ACCELERATION);
          conf.setKinematics(Constants.Drivetrain.KINEMATICS);
          Trajectory trajectory =
              TrajectoryGenerator.generateTrajectory(getPose(), List.of(), dest, conf);
          return makeTrajectoryCommand(trajectory);
        },
        this);
  }

  public Command makePositionCommand(Translation2d dest) {
    return new SuppliedCommand(
        () -> {
          TrajectoryConfig conf =
              new TrajectoryConfig(Constants.Auto.MAX_VELOCITY, Constants.Auto.MAX_ACCELERATION);
          Trajectory trajectory =
              TrajectoryGenerator.generateTrajectory(
                  new Pose2d(
                      getPose().getTranslation(),
                      dest.minus(getPose().getTranslation()).getAngle()),
                  List.of(getPose().getTranslation().interpolate(dest, 0.5)),
                  new Pose2d(dest, dest.minus(getPose().getTranslation()).getAngle()),
                  conf);
          return makeTrajectoryCommand(trajectory);
        },
        this);
  }

  public void setPos(Pose2d pos) {
    poseEstimator.resetPosition(getNavxRotation(), getPositions(), pos);
  }
}
