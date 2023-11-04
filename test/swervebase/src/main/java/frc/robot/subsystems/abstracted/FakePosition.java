package frc.robot.subsystems.abstracted;

public class FakePosition implements PositionMotor {
  double internal_pos;

  public FakePosition() {}

  @Override
  public void setPos(double pos) {
    internal_pos = pos;
  }

  @Override
  public double getPos() {
    return internal_pos;
  }
}
