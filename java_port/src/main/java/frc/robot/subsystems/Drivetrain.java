package frc.robot.subsystems;

import com.kauailabs.navx.frc.AHRS;
import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel;
import edu.wpi.first.wpilibj.SPI;
import edu.wpi.first.wpilibj2.command.SubsystemBase;
import frc.robot.Constants;

public class Drivetrain extends SubsystemBase {
  CANSparkMax fl = new CANSparkMax(Constants.IO.FL_ID, CANSparkMaxLowLevel.MotorType.kBrushless);
  CANSparkMax fr = new CANSparkMax(Constants.IO.FR_ID, CANSparkMaxLowLevel.MotorType.kBrushless);
  CANSparkMax bl = new CANSparkMax(Constants.IO.BL_ID, CANSparkMaxLowLevel.MotorType.kBrushless);
  CANSparkMax br = new CANSparkMax(Constants.IO.BR_ID, CANSparkMaxLowLevel.MotorType.kBrushless);

  AHRS navx = new AHRS(SPI.Port.kMXP);

  public Drivetrain() {
    navx.reset();
  }
}
