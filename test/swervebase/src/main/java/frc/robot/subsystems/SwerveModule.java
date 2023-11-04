package frc.robot.subsystems;

import com.revrobotics.*;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.kinematics.SwerveModulePosition;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import frc.robot.SwerveModuleConstants;
import frc.robot.subsystems.abstracted.*;

public class SwerveModule {
  SwerveModuleConstants constants;
  PositionMotor steer;
  VelocityMotor thrust;

  public SwerveModule(SwerveModuleConstants moduleConstants) {
    constants = moduleConstants;
    if (constants.steerID > 0) {
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
              constants.thrustKV,
              constants.thrustP,
              constants.thrustI,
              constants.thrustD);

    } else {
      steer = new FakePosition(String.valueOf(constants.steerID));
      thrust = new FakeVelocity(String.valueOf(constants.steerID));
    }
  }

  public void setModuleState(SwerveModuleState cmd_state) {
    SwerveModuleState state = SwerveModuleState.optimize(cmd_state, getModulePosition().angle);
    //state.speedMetersPerSecond *= getModulePosition().angle.minus(state.angle).getCos();
    steer.setPos(state.angle.unaryMinus().getRotations() + constants.steerOffsetRotations);
    thrust.setVelocity(0.1 * state.speedMetersPerSecond / constants.thrustDistancePerTick);
  }

  public SwerveModulePosition getModulePosition() {
    return new SwerveModulePosition(
        thrust.getPos() * constants.thrustDistancePerTick,
        Rotation2d.fromRotations(steer.getPos() - constants.steerOffsetRotations).unaryMinus());
  }
}
