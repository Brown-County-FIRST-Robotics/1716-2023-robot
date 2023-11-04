package frc.robot.subsystems.abstracted;

import edu.wpi.first.networktables.GenericEntry;
import edu.wpi.first.networktables.NetworkTableValue;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;

public class FakeVelocity implements VelocityMotor {
  private final String name;
  private final GenericEntry entri;
  Timer tm = new Timer();
  double pos = 0;
  double lastVel = 0;

  public FakeVelocity(String name_) {
    name = name_;
    entri = Shuffleboard.getTab("Logging").add(name + "_vel", 0.0).getEntry();
  }

  @Override
  public void setVelocity(double vel) {
    pos += tm.get() * lastVel;
    lastVel = vel * 10.0;
    entri.set(NetworkTableValue.makeDouble(lastVel / 2048.0));
    tm.reset();
    tm.start();
  }

  @Override
  public double getPos() {
    return pos;
  }
}
