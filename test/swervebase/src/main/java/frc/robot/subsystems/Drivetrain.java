package frc.robot.subsystems;

import com.kauailabs.navx.frc.AHRS;
import edu.wpi.first.math.controller.PIDController;
import edu.wpi.first.math.controller.ProfiledPIDController;
import edu.wpi.first.math.estimator.SwerveDrivePoseEstimator;
import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.kinematics.ChassisSpeeds;
import edu.wpi.first.math.kinematics.SwerveDriveKinematics;
import edu.wpi.first.math.kinematics.SwerveModulePosition;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import edu.wpi.first.math.trajectory.Trajectory;
import edu.wpi.first.math.trajectory.TrajectoryConfig;
import edu.wpi.first.math.trajectory.TrajectoryGenerator;
import edu.wpi.first.math.trajectory.TrapezoidProfile;
import edu.wpi.first.networktables.GenericEntry;
import edu.wpi.first.networktables.NetworkTableValue;
import edu.wpi.first.wpilibj.SPI;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.smartdashboard.Field2d;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.SubsystemBase;
import edu.wpi.first.wpilibj2.command.SwerveControllerCommand;
import frc.robot.Constants;
import java.util.List;

public class Drivetrain extends SubsystemBase {
  private final GenericEntry reset_button;
  SwerveModule fl = new SwerveModule(Constants.Drivetrain.FL);
  SwerveModule fr = new SwerveModule(Constants.Drivetrain.FR);
  SwerveModule bl = new SwerveModule(Constants.Drivetrain.BL);
  SwerveModule br = new SwerveModule(Constants.Drivetrain.BR);

  AHRS navx = new AHRS(SPI.Port.kMXP);

  SwerveDrivePoseEstimator poseEstimator;
  Field2d field;

  public SwerveModulePosition[] getPositions() {
    return new SwerveModulePosition[] {
      fl.getModulePosition(), fr.getModulePosition(), bl.getModulePosition(), br.getModulePosition()
    };
  }

  public Drivetrain() {
    field = new Field2d();
    navx.reset();
    poseEstimator =
        new SwerveDrivePoseEstimator(
            Constants.Drivetrain.KINEMATICS,
            getNavxRotation(),
            getPositions(),
            Constants.INIT_POSE);
    Shuffleboard.getTab("Debug").add("Pos sort of", field);
    reset_button = Shuffleboard.getTab("Teleop").add("Reset GYro", false).getEntry();
  }

  @Override
  public void periodic() {
    poseEstimator.update(getNavxRotation(), getPositions());
    field.setRobotPose(getPose());
    if (reset_button.getBoolean(false)) {
      setPos(new Pose2d(getPose().getTranslation(), new Rotation2d(0)));
      reset_button.set(NetworkTableValue.makeBoolean(false));
    }
  }

  public Pose2d getPose() {
    return poseEstimator.getEstimatedPosition();
  }

  public Rotation2d getNavxRotation() {
    return navx.getRotation2d();
  }

  // Field-oriented drive (units are meters/second)
  public void drive(double x, double y, double theta, boolean foc) {
    ChassisSpeeds sp;
    if (foc) {
      sp = ChassisSpeeds.fromFieldRelativeSpeeds(-x, -y, -theta, getPose().getRotation());
    } else {
      sp = new ChassisSpeeds(-x, -y, -theta);
    }
    setModuleStates(Constants.Drivetrain.KINEMATICS.toSwerveModuleStates(sp));
  }

  public void setModuleStates(SwerveModuleState[] states) {
    SwerveDriveKinematics.desaturateWheelSpeeds(states, Constants.Drivetrain.MAX_WHEEL_SPEED);
    fl.setModuleState(states[0]);
    fr.setModuleState(states[1]);
    bl.setModuleState(states[2]);
    br.setModuleState(states[3]);
  }

  public Command makeTrajectoryCommand(Trajectory trajectory) {
    return new SwerveControllerCommand(
        trajectory,
        this::getPose,
        Constants.Drivetrain.KINEMATICS,
        new PIDController(1, 1, 1),
        new PIDController(1, 1, 1),
        new ProfiledPIDController(1, 1, 1, new TrapezoidProfile.Constraints(1, 1)),
        this::setModuleStates,
        this);
  }

  public Command makePositionCommand(Pose2d dest) {
    TrajectoryConfig conf =
        new TrajectoryConfig(Constants.Auto.MAX_VELOCITY, Constants.Auto.MAX_ACCELERATION);
    conf.setKinematics(Constants.Drivetrain.KINEMATICS);
    Trajectory trajectory =
        TrajectoryGenerator.generateTrajectory(getPose(), List.of(), dest, conf);
    return makeTrajectoryCommand(trajectory);
  }

  public void setPos(Pose2d pos) {
    poseEstimator.resetPosition(getNavxRotation(), getPositions(), pos);
  }
}
