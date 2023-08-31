package frc.robot.subsystems;

import com.kauailabs.navx.frc.AHRS;
import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel;
import com.revrobotics.RelativeEncoder;
import edu.wpi.first.math.estimator.MecanumDrivePoseEstimator;
import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.kinematics.MecanumDriveWheelPositions;
import edu.wpi.first.wpilibj.DoubleSolenoid;
import edu.wpi.first.wpilibj.PneumaticHub;
import edu.wpi.first.wpilibj.SPI;
import edu.wpi.first.wpilibj2.command.SubsystemBase;
import frc.robot.Constants;

public class Drivetrain extends SubsystemBase {
  CANSparkMax fl = new CANSparkMax(Constants.IO.FL_ID, CANSparkMaxLowLevel.MotorType.kBrushless);
  CANSparkMax fr = new CANSparkMax(Constants.IO.FR_ID, CANSparkMaxLowLevel.MotorType.kBrushless);
  CANSparkMax bl = new CANSparkMax(Constants.IO.BL_ID, CANSparkMaxLowLevel.MotorType.kBrushless);
  CANSparkMax br = new CANSparkMax(Constants.IO.BR_ID, CANSparkMaxLowLevel.MotorType.kBrushless);

  RelativeEncoder flEncoder = fl.getEncoder();
  RelativeEncoder frEncoder = fr.getEncoder();
  RelativeEncoder blEncoder = bl.getEncoder();
  RelativeEncoder brEncoder = br.getEncoder();

  int waitTicksNeeded = -1;
  DoubleSolenoid solenoid;
  DoubleSolenoid.Value solenoidPos = DoubleSolenoid.Value.kReverse;

  AHRS navx = new AHRS(SPI.Port.kMXP);

  MecanumDrivePoseEstimator poseEstimator =
      new MecanumDrivePoseEstimator(
          Constants.DRIVETRAIN.KINEMATICS,
          navx.getRotation2d(),
          getEncoderPositions(),
          Constants.DRIVETRAIN.INIT_POSE);

  public Drivetrain(PneumaticHub hub) {
    navx.reset();
    fl.setInverted(false);
    fr.setInverted(true);
    bl.setInverted(false);
    br.setInverted(true);

    fl.burnFlash();
    fr.burnFlash();
    bl.burnFlash();
    br.burnFlash();
    solenoid =
        hub.makeDoubleSolenoid(
            Constants.IO.DRIVETRAIN_SOLENOID_ID[0], Constants.IO.DRIVETRAIN_SOLENOID_ID[1]);
  }

  public void setSolenoidPos(DoubleSolenoid.Value pos) {
    solenoid.set(pos);
    solenoidPos = pos;
    waitTicksNeeded = Constants.DRIVETRAIN.SOLENOID_WAIT_TICKS;
  }

  public void toggleSolenoidPos() {
    if (solenoidPos == DoubleSolenoid.Value.kReverse) {
      setSolenoidPos(DoubleSolenoid.Value.kForward);
    } else {
      setSolenoidPos(DoubleSolenoid.Value.kReverse);
    }
  }

  public DoubleSolenoid.Value getSolenoidPos() {
    return solenoidPos;
  }

  public void Periodic() {
    if (waitTicksNeeded == 0) {
      solenoid.set(DoubleSolenoid.Value.kOff);
    }
    waitTicksNeeded--;
    poseEstimator.update(navx.getRotation2d(), getEncoderPositions());
  }

  public Pose2d getPose() {
    return poseEstimator.getEstimatedPosition();
  }

  public MecanumDriveWheelPositions getEncoderPositions() {
    return new MecanumDriveWheelPositions(
        flEncoder.getPosition() / Constants.DRIVETRAIN.EFFECTIVE_WHEEL_DIAMETER,
        frEncoder.getPosition() / Constants.DRIVETRAIN.EFFECTIVE_WHEEL_DIAMETER,
        blEncoder.getPosition() / Constants.DRIVETRAIN.EFFECTIVE_WHEEL_DIAMETER,
        brEncoder.getPosition() / Constants.DRIVETRAIN.EFFECTIVE_WHEEL_DIAMETER);
  }
}
