package frc.robot.subsystems.drive;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel;
import com.revrobotics.SparkMaxAnalogSensor;
import com.revrobotics.SparkMaxPIDController;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.kinematics.SwerveModulePosition;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import frc.robot.SwerveModuleConstants;

public class ModuleIOSparkFX implements ModuleIO {
  private final CANSparkMax steer;
  private final SparkMaxAnalogSensor encoder;
  private final SparkMaxPIDController pid;
  private final WPI_TalonFX thrust;
  SwerveModuleConstants constants; // We should replace this with on-the-fly tunable numbers

  public ModuleIOSparkFX(SwerveModuleConstants constants) {
    this.constants = constants;
    thrust = new WPI_TalonFX(constants.thrustID);
    thrust.setNeutralMode(NeutralMode.Coast);
    thrust.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 20);
    thrust.configNominalOutputForward(0, 20);
    thrust.configNominalOutputReverse(0, 20);
    thrust.configPeakOutputForward(1, 20);
    thrust.configPeakOutputReverse(-1, 20);
    thrust.config_kP(0, constants.thrustP, 20);
    thrust.config_kI(0, constants.thrustI, 20);
    thrust.config_kD(0, constants.thrustD, 20);
    thrust.config_kF(0, constants.thrustKV, 20);

    steer = new CANSparkMax(constants.steerID, CANSparkMaxLowLevel.MotorType.kBrushless);
    steer.restoreFactoryDefaults();
    pid = steer.getPIDController();
    encoder = steer.getAnalog(SparkMaxAnalogSensor.Mode.kAbsolute);

    encoder.setPositionConversionFactor(1 / 3.3);
    encoder.setInverted(true);
    pid.setFeedbackDevice(encoder);

    pid.setP(constants.steerP);
    pid.setI(constants.steerI);
    pid.setD(constants.steerD);
    pid.setFF(constants.steerKV);
    pid.setOutputRange(-1, 1);
    pid.setSmartMotionMaxVelocity(300, 0);
    pid.setSmartMotionMinOutputVelocity(0, 0);
    pid.setSmartMotionMaxAccel(1200, 0);
    pid.setSmartMotionAllowedClosedLoopError(0.01, 0);
    pid.setPositionPIDWrappingEnabled(true);
    pid.setPositionPIDWrappingMaxInput(1);
    pid.setPositionPIDWrappingMinInput(0);
    steer.setSmartCurrentLimit(30);
    steer.burnFlash();
  }

  @Override
  public void updateInputs(ModuleIOInputs inputs) {
    inputs.steerPos = getModulePosition().angle.getRotations();
    inputs.thrustPos = getModulePosition().distanceMeters;
    inputs.steerTempC = steer.getMotorTemperature();
    inputs.thrustErr = thrust.getClosedLoopError();
    inputs.thrustTempC = thrust.getTemperature();
  }

  @Override
  public void setCmdState(SwerveModuleState cmd_state) {
    SwerveModuleState state =
        SwerveModuleState.optimize(
            cmd_state, getModulePosition().angle.plus(Rotation2d.fromRotations(1.0)));
    state.speedMetersPerSecond *= getModulePosition().angle.minus(state.angle).getCos();
    double cmd_ang = state.angle.unaryMinus().getRotations() + constants.steerOffsetRotations;
    thrust.set(
        TalonFXControlMode.Velocity,
        0.1 * state.speedMetersPerSecond / constants.thrustDistancePerTick);

    pid.setReference(((cmd_ang % 1.0) + 1.0) % 1.0, CANSparkMax.ControlType.kSmartMotion);
  }

  private SwerveModulePosition getModulePosition() {
    return new SwerveModulePosition(
        thrust.getSelectedSensorPosition() * constants.thrustDistancePerTick,
        Rotation2d.fromRotations(encoder.getPosition() - constants.steerOffsetRotations)
            .unaryMinus());
  }
}
