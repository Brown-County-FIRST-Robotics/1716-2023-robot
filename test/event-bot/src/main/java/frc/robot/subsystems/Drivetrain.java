package frc.robot.subsystems;


import com.kauailabs.navx.frc.AHRS;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.drive.MecanumDrive;
import edu.wpi.first.wpilibj2.command.SubsystemBase;
import edu.wpi.first.wpilibj.SPI;
import edu.wpi.first.wpilibj.DriverStation;

import frc.robot.Constants.DrivetrainConst;

public class Drivetrain extends SubsystemBase {
	private final WPI_TalonFX frontLeft = new WPI_TalonFX(DrivetrainConst.FrontLeftMotorPort);
	private final WPI_TalonFX rearLeft = new WPI_TalonFX(DrivetrainConst.RearLeftMotorPort);
	private final WPI_TalonFX frontRight = new WPI_TalonFX(DrivetrainConst.FrontRightMotorPort);
	private final WPI_TalonFX rearRight = new WPI_TalonFX(DrivetrainConst.RearRightMotorPort);

	private final MecanumDrive mecanumDrive = new MecanumDrive(frontLeft, rearLeft, frontRight, rearRight);

	AHRS navX;

	public Drivetrain() {
		frontRight.setInverted(true);
		rearRight.setInverted(true);

		try {
			navX = new AHRS(SPI.Port.kMXP);
		}
		catch (RuntimeException ex) {
			DriverStation.reportError("Error instantiating navX MXP:  " + ex.getMessage(), true);
		}
	}

	/**
	 * Drives the robot at given x, y and rotational speeds. Speeds range from [-1, 1].
	 *
	 * @param xSpeed Speed of the robot in the x direction (forwards/backwards).
	 * @param ySpeed Speed of the robot in the y direction (sideways).
	 * @param rot Rotational rate of the robot.
	 */
	public void drive(double xSpeed, double ySpeed, double rot) {
		mecanumDrive.driveCartesian(xSpeed * DrivetrainConst.MotorMaxSpeed, ySpeed * DrivetrainConst.MotorMaxSpeed, rot * DrivetrainConst.MotorMaxSpeed,
			navX.getRotation2d());
	}

	public void setNeutralMode(NeutralMode neutralMode) { //brake/coast mode
		frontLeft.setNeutralMode(neutralMode);
		frontRight.setNeutralMode(neutralMode);
		rearLeft.setNeutralMode(neutralMode);
		rearRight.setNeutralMode(neutralMode);
	}
}
