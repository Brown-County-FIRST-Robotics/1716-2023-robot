package frc.robot.subsystems;

import com.kauailabs.navx.frc.AHRS;

import java.util.ArrayList;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.can.TalonFX;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.music.Orchestra;

import edu.wpi.first.wpilibj.drive.MecanumDrive;
import edu.wpi.first.wpilibj2.command.SubsystemBase;
import edu.wpi.first.wpilibj.SPI;
import edu.wpi.first.wpilibj.DriverStation;

import frc.robot.Constants.DrivetrainConst;

public class Drivetrain extends SubsystemBase {
	private double maxSpeed = 0.3;

	private final WPI_TalonFX frontLeft = new WPI_TalonFX(DrivetrainConst.FrontLeftMotorPort);
	private final WPI_TalonFX backLeft = new WPI_TalonFX(DrivetrainConst.BackLeftMotorPort);
	private final WPI_TalonFX frontRight = new WPI_TalonFX(DrivetrainConst.FrontRightMotorPort);
	private final WPI_TalonFX backRight = new WPI_TalonFX(DrivetrainConst.BackRightMotorPort);

	private final MecanumDrive mecanumDrive = new MecanumDrive(frontLeft, backLeft, frontRight, backRight);

	AHRS navX;

	boolean doFieldOriented = false;
	NeutralMode neutralMode = NeutralMode.Coast;

	//music playing:
	Orchestra orchestra;

	public Drivetrain() {
		frontRight.setInverted(true);
		frontLeft.setInverted(false);
		backRight.setInverted(true);
		backLeft.setInverted(false);

		neutralMode = NeutralMode.Brake;
		toggleNeutralMode();

		try {
			navX = new AHRS(SPI.Port.kMXP);
		}
		catch (RuntimeException ex) {
			DriverStation.reportError("Error instantiating navX MXP:  " + ex.getMessage(), true);
		}

		//Music playing:
		frontLeft.setSafetyEnabled(false);
		frontRight.setSafetyEnabled(false);
		backLeft.setSafetyEnabled(false);
		backRight.setSafetyEnabled(false);
		mecanumDrive.setSafetyEnabled(false);
        //orchestra initialization
        ArrayList<TalonFX> instruments = new ArrayList<TalonFX>();
        instruments.add(frontLeft);
		instruments.add(frontRight);
		instruments.add(backLeft);
		instruments.add(backRight);
        orchestra = new Orchestra(instruments);
	}

	/**
	 * Drives the robot at given x, y and rotational speeds. Speeds range from [-1, 1].
	 *
	 * @param xSpeed Speed of the robot in the x direction (forwards/backwards).
	 * @param ySpeed Speed of the robot in the y direction (sideways).
	 * @param rot Rotational rate of the robot.
	 */
	public void drive(double xSpeed, double ySpeed, double rot) {
		if (xSpeed == 0 && ySpeed == 0 && rot == 0)
			return;
		if (orchestra.isPlaying())
			stopSong();
		if (doFieldOriented)
			mecanumDrive.driveCartesian(xSpeed * maxSpeed, ySpeed * maxSpeed, rot * maxSpeed,
				navX.getRotation2d().times(-1));
		else
			mecanumDrive.driveCartesian(xSpeed * maxSpeed, ySpeed * maxSpeed, rot * maxSpeed);
	}

	public void incrementMaxSpeed() {
		maxSpeed += 0.1;
		if (maxSpeed >= 1) {
			maxSpeed = 1;
		}
	}

	public void decrementMaxSpeed() {
		maxSpeed -= 0.1;
		if (maxSpeed <= 0) {
			maxSpeed = 0;
		}
	}

	public void toggleNeutralMode() {
		if (neutralMode == NeutralMode.Brake) {
			frontLeft.setNeutralMode(NeutralMode.Coast);
			frontRight.setNeutralMode(NeutralMode.Coast);
			backLeft.setNeutralMode(NeutralMode.Coast);
			backRight.setNeutralMode(NeutralMode.Coast);
			neutralMode = NeutralMode.Coast;
		}
		else {
			frontLeft.setNeutralMode(NeutralMode.Brake);
			frontRight.setNeutralMode(NeutralMode.Brake);
			backLeft.setNeutralMode(NeutralMode.Brake);
			backRight.setNeutralMode(NeutralMode.Brake);
			neutralMode = NeutralMode.Brake;
		}
	}
	public NeutralMode getNeutralMode() {
		return neutralMode;
	}

	public void toggleFieldOriented() {
		doFieldOriented = !doFieldOriented;
	}

	public void resetGyroscope() {
		navX.reset();
	}

	public void playSong(String filepath) {
		if (orchestra.isPlaying()) {
			orchestra.stop();
		}
		orchestra.loadMusic(filepath);
		orchestra.play();
	}
	public void playSong() {
		orchestra.play();
	}
	public void pauseSong() {
		orchestra.pause();
	}
	public void stopSong() {
		orchestra.stop();
	}
}
