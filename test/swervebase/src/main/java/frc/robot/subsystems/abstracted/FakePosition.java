package frc.robot.subsystems.abstracted;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class FakePosition implements PositionMotor {
  double internal_pos;
  private String name;

  public FakePosition(String name_) {
    name = name_;
  }

  @Override
  public void setPos(double pos) {
    internal_pos = pos;
    SmartDashboard.putNumber(name + "_pos", pos);
  }

  @Override
  public double getPos() {
    return internal_pos;
  }
}
