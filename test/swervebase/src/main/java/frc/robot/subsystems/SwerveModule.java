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
    steerPID = steer.getPIDController();
    analogSensor = steer.getAnalog(SparkMaxAnalogSensor.Mode.kAbsolute);
    analogSensor.setPositionConversionFactor(1 / 3.3);

    steerPID.setP(constants.steerP);
    steerPID.setI(constants.steerI);
    steerPID.setD(constants.steerD);
    steerPID.setFF(constants.steerKV);
    steer.burnFlash();

    thrust.configFactoryDefault();
    thrust.setNeutralMode(NeutralMode.Brake);
    thrust.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 20);
    thrust.configNominalOutputForward(0, 200);
    thrust.configNominalOutputReverse(0, 200);
    thrust.configPeakOutputForward(1, 200);
    thrust.configPeakOutputReverse(-1, 200);
    thrust.config_kP(0, constants.thrustP, 200);
    thrust.config_kI(0, constants.thrustI, 200);
    thrust.config_kD(0, constants.thrustD, 200);
    thrust.config_kF(0, constants.thrustKV, 200);
  }

  public void setModuleState(SwerveModuleState cmd_state) {
    SwerveModuleState state = SwerveModuleState.optimize(cmd_state, getModulePosition().angle);
    state.speedMetersPerSecond *= getModulePosition().angle.minus(state.angle).getCos();
    steerPID.setReference(
        state.angle.getRotations() - constants.steerOffsetRotations,
        CANSparkMax.ControlType.kPosition);
    thrust.set(
        TalonFXControlMode.Velocity,
        0.1 * state.speedMetersPerSecond / constants.thrustDistancePerTick);
  }

  public SwerveModulePosition getModulePosition() {
    return new SwerveModulePosition(
        thrust.getSelectedSensorPosition() * constants.thrustDistancePerTick,
        Rotation2d.fromRotations(analogSensor.getPosition() + constants.steerOffsetRotations));
  }
}
