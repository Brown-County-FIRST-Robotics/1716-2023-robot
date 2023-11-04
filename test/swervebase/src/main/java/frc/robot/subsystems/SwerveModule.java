package frc.robot.subsystems;

import com.revrobotics.*;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.kinematics.SwerveModulePosition;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import frc.robot.SwerveModuleConstants;
import frc.robot.subsystems.abstracted.VelocityMotor;
import frc.robot.subsystems.abstracted.VelocityTalon;

public class SwerveModule {
  SwerveModuleConstants constants;
  CANSparkMax steer;
  VelocityMotor thrust;
  SparkMaxAnalogSensor analogSensor;
  SparkMaxPIDController steerPID;

  public SwerveModule(SwerveModuleConstants moduleConstants) {
    constants = moduleConstants;
    steer = new CANSparkMax(constants.steerID, CANSparkMaxLowLevel.MotorType.kBrushless);
    thrust =
        new VelocityTalon(
            constants.thrustID,
            constants.steerKV,
            constants.thrustP,
            constants.thrustI,
            constants.thrustD);
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
    steerPID.setOutputRange(-1, 1);
    steerPID.setSmartMotionMaxVelocity(300, 0);
    steerPID.setSmartMotionMinOutputVelocity(0, 0);
    steerPID.setSmartMotionMaxAccel(300, 0);
    steerPID.setSmartMotionAllowedClosedLoopError(0.005, 0);
    steer.burnFlash();
  }

  public void setModuleState(SwerveModuleState cmd_state) {
    SwerveModuleState state = SwerveModuleState.optimize(cmd_state, getModulePosition().angle);
    state.speedMetersPerSecond *= getModulePosition().angle.minus(state.angle).getCos();
    steerPID.setReference(
        state.angle.getRotations() + constants.steerOffsetRotations,
        CANSparkMax.ControlType.kSmartMotion);
    thrust.setVelocity(0.1 * state.speedMetersPerSecond / constants.thrustDistancePerTick);
  }

  public SwerveModulePosition getModulePosition() {
    return new SwerveModulePosition(
        thrust.getPos() * constants.thrustDistancePerTick,
        Rotation2d.fromRotations(analogSensor.getPosition() - constants.steerOffsetRotations));
  }
}
