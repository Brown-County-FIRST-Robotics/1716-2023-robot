package frc.robot.subsystems.abstracted;

import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class FakeVelocity implements VelocityMotor {
  private final String name;
  Timer tm = new Timer();
  double pos = 0;
  double lastVel = 0;

  public FakeVelocity(String name_) {
    name = name_;
  }

  @Override
  public void setVelocity(double vel) {
    pos += tm.get() * lastVel;
    lastVel = vel * 10;
    SmartDashboard.putNumber(name + "_vel", lastVel);
    tm.reset();
    tm.start();
  }

  @Override
  public double getPos() {
    return pos;
  }
}
