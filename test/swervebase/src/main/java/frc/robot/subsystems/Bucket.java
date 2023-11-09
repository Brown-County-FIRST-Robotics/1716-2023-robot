package frc.robot.subsystems;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import edu.wpi.first.wpilibj.Encoder;
import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class Bucket extends SubsystemBase {
  // const, move to constant file eventually
  private double P = 0.1;
  private double DOWN_POS = 0.0;

  private final WPI_TalonSRX motor = new WPI_TalonSRX(0);
  private final Encoder encoder = new Encoder(0, 1);

  boolean extended = false;

  public Bucket() {
    motor.setInverted(false);
    motor.setNeutralMode(NeutralMode.Brake);

    encoder.reset();
    encoder.setReverseDirection(false);
  }

  public void SetPosition(boolean extend) {
    extended = extend;
  }

  @Override
  public void periodic() {
    if (extended == false && encoder.getDistance() != 0) {
      motor.set(P * Math.abs(encoder.getDistance()));
    } else if (extended == true && encoder.getDistance() != DOWN_POS) {
      motor.set(P * Math.abs(encoder.getDistance() - DOWN_POS));
    } else {
      motor.set(0);
    }
  }
}
