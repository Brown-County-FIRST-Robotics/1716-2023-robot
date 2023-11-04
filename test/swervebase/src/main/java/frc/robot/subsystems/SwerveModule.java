package frc.robot.subsystems;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.revrobotics.*;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.kinematics.SwerveModulePosition;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import frc.robot.SwerveModuleConstants;

public class SwerveModule {
  SwerveModuleConstants constants;
  CANSparkMax steer;
  WPI_TalonFX thrust;
  SparkMaxAnalogSensor analogSensor;
  SparkMaxPIDController steerPID;

  public SwerveModule(SwerveModuleConstants moduleConstants) {
    constants = moduleConstants;
    steer = new CANSparkMax(constants.steerID, CANSparkMaxLowLevel.MotorType.kBrushless);
    thrust = new WPI_TalonFX(constants.thrustID);
    steer.restoreFactoryDefaults();
    steerPID = steer.getPIDController();
    analogSensor = steer.getAnalog(SparkMaxAnalogSensor.Mode.kAbsolute);
    analogSensor.setPositionConversionFactor(1 / 3.3);
    analogSensor.setInverted(true);
    steerPID.setFeedbackDevice(analogSensor);

    steerPID.setP(constants.steerP);
    steerPID.setI(constants.steerI);
    steerPID.setD(constants.steerD);
    steerPID.setFF(constants.steerKV);
    steerPID.setOutputRange(-1,1);
    steerPID.setSmartMotionMaxVelocity(300,0);
    steerPID.setSmartMotionMinOutputVelocity(0,0);
    steerPID.setSmartMotionMaxAccel(300,0);
    steerPID.setSmartMotionAllowedClosedLoopError(0.005,0);
    steer.burnFlash();

    thrust.setNeutralMode(NeutralMode.Brake);
    thrust.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 20);
    thrust.configNominalOutputForward(0, 20);
    thrust.configNominalOutputReverse(0, 20);
    thrust.configPeakOutputForward(1, 20);
    thrust.configPeakOutputReverse(-1, 20);
    thrust.config_kP(0, constants.thrustP, 20);
    thrust.config_kI(0, constants.thrustI, 20);
    thrust.config_kD(0, constants.thrustD, 20);
    thrust.config_kF(0, constants.thrustKV, 20);
  }

  public void setModuleState(SwerveModuleState cmd_state) {
    SwerveModuleState state = SwerveModuleState.optimize(cmd_state, getModulePosition().angle);
    state.speedMetersPerSecond *= getModulePosition().angle.minus(state.angle).getCos();
    steerPID.setReference(
        state.angle.getRotations() + constants.steerOffsetRotations,
        CANSparkMax.ControlType.kSmartMotion);
    thrust.set(
        TalonFXControlMode.Velocity,
        0.1 * state.speedMetersPerSecond / constants.thrustDistancePerTick);
  }

  public SwerveModulePosition getModulePosition() {
    return new SwerveModulePosition(
        thrust.getSelectedSensorPosition() * constants.thrustDistancePerTick,
        Rotation2d.fromRotations(analogSensor.getPosition() - constants.steerOffsetRotations));
  }
}
