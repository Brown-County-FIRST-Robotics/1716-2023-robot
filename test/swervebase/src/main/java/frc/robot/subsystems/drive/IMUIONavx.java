package frc.robot.subsystems.drive;

import com.kauailabs.navx.frc.AHRS;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.wpilibj.SPI;

public class IMUIONavx implements IMUIO {
  AHRS imu;

  public IMUIONavx() {
    imu = new AHRS(SPI.Port.kMXP);
  }

  @Override
  public void updateInputs(IMUIOInputs inputs) {
    inputs.tempC = imu.getTempC();
    inputs.pitch = Rotation2d.fromDegrees(imu.getPitch());
    inputs.yaw = Rotation2d.fromDegrees(imu.getYaw());
    inputs.roll = Rotation2d.fromDegrees(imu.getRoll());
    inputs.xAccelMPS = imu.getRawAccelX() * 9.8065;
    inputs.yAccelMPS = imu.getRawAccelY() * 9.8065;
    inputs.zAccelMPS = imu.getRawAccelZ() * 9.8065;
  }
}
