package frc.robot.subsystems.abstracted;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import edu.wpi.first.math.filter.SlewRateLimiter;

public class VelocityTalon implements VelocityMotor {
  private final WPI_TalonFX motor;
  private final SlewRateLimiter lim=new SlewRateLimiter(12000*3); // units/100ms
  //

  public VelocityTalon(int CANID, double KV, double p, double i, double d) {
    motor = new WPI_TalonFX(CANID);
    motor.setNeutralMode(NeutralMode.Coast);
    motor.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 20);
    motor.configNominalOutputForward(0, 20);
    motor.configNominalOutputReverse(0, 20);
    motor.configPeakOutputForward(1, 20);
    motor.configPeakOutputReverse(-1, 20);
    motor.config_kP(0, p, 20);
    motor.config_kI(0, i, 20);
    motor.config_kD(0, d, 20);
    motor.config_kF(0, KV, 20);
  }

  @Override
  public void setVelocity(double vel) {
    motor.set(TalonFXControlMode.Velocity, lim.calculate(vel));
  }

  @Override
  public double getPos() {
    return motor.getSelectedSensorPosition();
  }
}
