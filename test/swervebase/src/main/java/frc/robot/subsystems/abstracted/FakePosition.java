package frc.robot.subsystems.abstracted;

import edu.wpi.first.networktables.GenericEntry;
import edu.wpi.first.networktables.NetworkTableValue;
import edu.wpi.first.wpilibj.shuffleboard.BuiltInWidgets;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;

public class FakePosition implements PositionMotor {
  double internal_pos;
  double cmdd_pos;
  double vel = 3;
  private String name;
  private GenericEntry entri;

  public FakePosition(String name_) {
    name = name_;
    entri =
        Shuffleboard.getTab("Logging")
            .add(name + "_pos", 0.0)
            .withWidget(BuiltInWidgets.kDial)
            .getEntry();
  }

  @Override
  public void setPos(double pos) {
    internal_pos = pos;
    entri.set(NetworkTableValue.makeDouble(pos * 100.0));
  }

  @Override
  public double getPos() {
    return internal_pos;
  }
}
