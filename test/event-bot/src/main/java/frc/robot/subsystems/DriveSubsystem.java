package frc.robot.subsystems;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.drive.MecanumDrive;
import frc.robot.Constants.DriveConstants;
import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class DriveSubsystem extends SubsystemBase {
	private final WPI_TalonFX frontLeft = new WPI_TalonFX(DriveConstants.FrontLeftMotorPort);
	private final WPI_TalonFX rearLeft = new WPI_TalonFX(DriveConstants.RearLeftMotorPort);
	private final WPI_TalonFX frontRight = new WPI_TalonFX(DriveConstants.FrontRightMotorPort);
	private final WPI_TalonFX rearRight = new WPI_TalonFX(DriveConstants.RearRightMotorPort);

	private final MecanumDrive mecanumDrive = new MecanumDrive(frontLeft, rearLeft, frontRight, rearRight);

	public DriveSubsystem() {
		frontRight.setInverted(true);
		rearRight.setInverted(true);
	}

	/**
	 * Drives the robot at given x, y and rotational speeds. Speeds range from [-1, 1].
	 *
	 * @param xSpeed Speed of the robot in the x direction (forwards/backwards).
	 * @param ySpeed Speed of the robot in the y direction (sideways).
	 * @param rot Rotational rate of the robot.
	 */
	public void drive(double xSpeed, double ySpeed, double rot) {
		mecanumDrive.driveCartesian(xSpeed * DriveConstants.MotorMaxSpeed, ySpeed * DriveConstants.MotorMaxSpeed, rot * DriveConstants.MotorMaxSpeed);
	}
}
