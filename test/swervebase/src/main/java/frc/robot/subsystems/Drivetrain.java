package frc.robot.subsystems;

import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.kauailabs.navx.frc.AHRS;
import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel.MotorType;
import com.revrobotics.SparkMaxPIDController;
import edu.wpi.first.wpilibj.SPI;
import edu.wpi.first.wpilibj2.command.SubsystemBase;
import edu.wpi.first.math.estimator.SwerveDrivePoseEstimator;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import frc.robot.Constants;

public class Drivetrain extends SubsystemBase {
  WPI_TalonFX fl_drive = new WPI_TalonFX(Constants.IO.FL_DRIVE_ID);
  WPI_TalonFX fr_drive = new WPI_TalonFX(Constants.IO.FR_DRIVE_ID);
  WPI_TalonFX bl_drive = new WPI_TalonFX(Constants.IO.BL_DRIVE_ID);
  WPI_TalonFX br_drive = new WPI_TalonFX(Constants.IO.BR_DRIVE_ID);
  CANSparkMax fl_steer = new CANSparkMax(Constants.IO.FL_STEER_ID, MotorType.kBrushless);
  CANSparkMax fr_steer = new CANSparkMax(Constants.IO.FR_STEER_ID, MotorType.kBrushless);
  CANSparkMax bl_steer = new CANSparkMax(Constants.IO.BL_STEER_ID, MotorType.kBrushless);
  CANSparkMax br_steer = new CANSparkMax(Constants.IO.BR_STEER_ID, MotorType.kBrushless);
  SparkMaxPIDController fl_steer_pid = fl_steer.getPIDController();
  SparkMaxPIDController fr_steer_pid = fr_steer.getPIDController();
  SparkMaxPIDController bl_steer_pid = bl_steer.getPIDController();
  SparkMaxPIDController br_steer_pid = br_steer.getPIDController();

  AHRS navx = new AHRS(SPI.Port.kMXP);



  public void configure() {
    fl_steer_pid.setP(Constants.Drivetrain.FL_STEER_P);
    fl_steer_pid.setI(Constants.Drivetrain.FL_STEER_I);
    fl_steer_pid.setD(Constants.Drivetrain.FL_STEER_D);
    fl_steer_pid.setFF(Constants.Drivetrain.FL_STEER_FF);

    fr_steer_pid.setP(Constants.Drivetrain.FR_STEER_P);
    fr_steer_pid.setI(Constants.Drivetrain.FR_STEER_I);
    fr_steer_pid.setD(Constants.Drivetrain.FR_STEER_D);
    fr_steer_pid.setFF(Constants.Drivetrain.FR_STEER_FF);

    bl_steer_pid.setP(Constants.Drivetrain.BL_STEER_P);
    bl_steer_pid.setI(Constants.Drivetrain.BL_STEER_I);
    bl_steer_pid.setD(Constants.Drivetrain.BL_STEER_D);
    bl_steer_pid.setFF(Constants.Drivetrain.BL_STEER_FF);

    br_steer_pid.setP(Constants.Drivetrain.BR_STEER_P);
    br_steer_pid.setI(Constants.Drivetrain.BR_STEER_I);
    br_steer_pid.setD(Constants.Drivetrain.BR_STEER_D);
    br_steer_pid.setFF(Constants.Drivetrain.BR_STEER_FF);

    // Burn flash in case of a brownout
    fl_steer.burnFlash();
    fr_steer.burnFlash();
    bl_steer.burnFlash();
    br_steer.burnFlash();

    fl_drive.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 20);
    fr_drive.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 20);
    bl_drive.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 20);
    br_drive.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 20);

    fl_drive.config_kP(0, Constants.Drivetrain.FL_DRIVE_P, 20);
    fl_drive.config_kI(0, Constants.Drivetrain.FL_DRIVE_I, 20);
    fl_drive.config_kD(0, Constants.Drivetrain.FL_DRIVE_D, 20);
    fl_drive.config_kF(0, Constants.Drivetrain.FL_DRIVE_FF, 20);

    fr_drive.config_kP(0, Constants.Drivetrain.FR_DRIVE_P, 20);
    fr_drive.config_kI(0, Constants.Drivetrain.FR_DRIVE_I, 20);
    fr_drive.config_kD(0, Constants.Drivetrain.FR_DRIVE_D, 20);
    fr_drive.config_kF(0, Constants.Drivetrain.FR_DRIVE_FF, 20);

    bl_drive.config_kP(0, Constants.Drivetrain.BL_DRIVE_P, 20);
    bl_drive.config_kI(0, Constants.Drivetrain.BL_DRIVE_I, 20);
    bl_drive.config_kD(0, Constants.Drivetrain.BL_DRIVE_D, 20);
    bl_drive.config_kF(0, Constants.Drivetrain.BL_DRIVE_FF, 20);

    br_drive.config_kP(0, Constants.Drivetrain.BR_DRIVE_P, 20);
    br_drive.config_kI(0, Constants.Drivetrain.BR_DRIVE_I, 20);
    br_drive.config_kD(0, Constants.Drivetrain.BR_DRIVE_D, 20);
    br_drive.config_kF(0, Constants.Drivetrain.BR_DRIVE_FF, 20);
  }

  public Drivetrain() {
    navx.reset();
    configure();
  }

  public void setModuleStates(SwerveModuleState[] states) {
    fl_steer_pid.setReference(
        states[0].angle.getDegrees() / 360, CANSparkMax.ControlType.kPosition);
    fr_steer_pid.setReference(
        states[1].angle.getDegrees() / 360, CANSparkMax.ControlType.kPosition);
    bl_steer_pid.setReference(
        states[2].angle.getDegrees() / 360, CANSparkMax.ControlType.kPosition);
    br_steer_pid.setReference(
        states[3].angle.getDegrees() / 360, CANSparkMax.ControlType.kPosition);

    fl_drive.set(
        TalonFXControlMode.Velocity,
        states[0].speedMetersPerSecond / Constants.Drivetrain.EFFECTIVE_WHEEL_DIAMETER);
    fr_drive.set(
        TalonFXControlMode.Velocity,
        states[1].speedMetersPerSecond / Constants.Drivetrain.EFFECTIVE_WHEEL_DIAMETER);
    bl_drive.set(
        TalonFXControlMode.Velocity,
        states[2].speedMetersPerSecond / Constants.Drivetrain.EFFECTIVE_WHEEL_DIAMETER);
    br_drive.set(
        TalonFXControlMode.Velocity,
        states[3].speedMetersPerSecond / Constants.Drivetrain.EFFECTIVE_WHEEL_DIAMETER);
  }
}
