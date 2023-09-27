package frc.robot.subsystems;

import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.revrobotics.*;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.kinematics.SwerveModulePosition;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import edu.wpi.first.wpilibj.AnalogInput;
import frc.robot.Constants;

public class SwerveModule {
  Constants.SwerveModuleConstants constants;
  CANSparkMax steer;
  WPI_TalonFX thrust;
  RelativeEncoder steerEncoder;
  SparkMaxPIDController steerPID;
  AnalogInput steerAbsoluteEncoder;

  public SwerveModule(Constants.SwerveModuleConstants moduleConstants) {
    constants = moduleConstants;
    steer = new CANSparkMax(constants.steerID, CANSparkMaxLowLevel.MotorType.kBrushless);
    thrust = new WPI_TalonFX(constants.thrustID);
    steerEncoder = steer.getEncoder();
    steerPID = steer.getPIDController();
    steerAbsoluteEncoder = new AnalogInput(constants.encoderID);

    steerPID.setP(constants.steerP);
    steerPID.setI(constants.steerI);
    steerPID.setD(constants.steerD);
    steerPID.setFF(constants.steerKV);
    steer.burnFlash();
    steerEncoder.setPosition(constants.getRotationsFromAngle(getRawAbsoluteEncoder()));

    thrust.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 20);
    thrust.config_kP(0, constants.thrustP, 20);
    thrust.config_kI(0, constants.thrustI, 20);
    thrust.config_kD(0, constants.thrustD, 20);
    thrust.config_kF(0, constants.thrustKV, 20);
  }

  public void setModuleState(SwerveModuleState state) {
    steerPID.setReference(
        constants.getRotationsFromAngle(state.angle.getRadians()),
        CANSparkMax.ControlType.kSmartMotion);
    thrust.set(
        TalonFXControlMode.Velocity,
        10 * state.speedMetersPerSecond / constants.thrustDistancePerTick);
  }

  public SwerveModulePosition getModulePosition() {
    return new SwerveModulePosition(
        thrust.getSelectedSensorPosition() * constants.thrustDistancePerTick,
        Rotation2d.fromRadians(constants.getAngleFromRotations(steerEncoder.getPosition())));
  }

  public double getRawAbsoluteEncoder() {
    return 2 * Math.PI * steerAbsoluteEncoder.getVoltage() / 5;
  }

  public void updateRelativeEncoder() {
    steerEncoder.setPosition(constants.getRotationsFromAngle(getRawAbsoluteEncoder()));
  }
}
