package frc.robot.subsystems;

import com.revrobotics.*;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.kinematics.SwerveModulePosition;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import frc.robot.SwerveModuleConstants;
import frc.robot.subsystems.abstracted.PositionMotor;
import frc.robot.subsystems.abstracted.PositionSparkMax;
import frc.robot.subsystems.abstracted.VelocityMotor;
import frc.robot.subsystems.abstracted.VelocityTalon;

public class SwerveModule {
  SwerveModuleConstants constants;
  PositionMotor steer;
  VelocityMotor thrust;

  public SwerveModule(SwerveModuleConstants moduleConstants) {
    constants = moduleConstants;
    steer =
        new PositionSparkMax(
            constants.steerID,
            constants.steerKV,
            constants.steerP,
            constants.steerI,
            constants.steerD);
    thrust =
        new VelocityTalon(
            constants.thrustID,
            constants.steerKV,
            constants.thrustP,
            constants.thrustI,
            constants.thrustD);
  }

  public void setModuleState(SwerveModuleState cmd_state) {
    SwerveModuleState state = SwerveModuleState.optimize(cmd_state, getModulePosition().angle);
    state.speedMetersPerSecond *= getModulePosition().angle.minus(state.angle).getCos();
    steer.setPos(state.angle.getRotations() + constants.steerOffsetRotations);
    thrust.setVelocity(0.1 * state.speedMetersPerSecond / constants.thrustDistancePerTick);
  }

  public SwerveModulePosition getModulePosition() {
    return new SwerveModulePosition(
        thrust.getPos() * constants.thrustDistancePerTick,
        Rotation2d.fromRotations(thrust.getPos() - constants.steerOffsetRotations));
  }
}
