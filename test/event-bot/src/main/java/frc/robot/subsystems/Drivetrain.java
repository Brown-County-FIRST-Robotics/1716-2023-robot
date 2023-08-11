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
	private final WPI_TalonFX frontLeft = new WPI_TalonFX(DrivetrainConst.FrontLeftMotorPort);
	private final WPI_TalonFX backLeft = new WPI_TalonFX(DrivetrainConst.RearLeftMotorPort);
	private final WPI_TalonFX frontRight = new WPI_TalonFX(DrivetrainConst.FrontRightMotorPort);
	private final WPI_TalonFX backRight = new WPI_TalonFX(DrivetrainConst.RearRightMotorPort);

	private final MecanumDrive mecanumDrive = new MecanumDrive(frontLeft, backLeft, frontRight, backRight);

	AHRS navX;

	boolean doFieldOriented = false;
	NeutralMode neutralMode = NeutralMode.Coast;

	//music playing:
	Orchestra orchestra;
	// Orchestra frontLeftOrchestra;
	// Orchestra frontRightOrchestra;
	// Orchestra backLeftOrchestra;
	// Orchestra backRightOrchestra;


	public Drivetrain() {
		frontRight.setInverted(true);
		frontLeft.setInverted(true);
		backRight.setInverted(false);
		backLeft.setInverted(false);

		setNeutralMode(NeutralMode.Coast);

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
		// ArrayList<TalonFX> frontLeftInstrument = new ArrayList<TalonFX>();
		// frontLeftInstrument.add(frontLeft);
		// frontLeftOrchestra = new Orchestra(frontLeftInstrument);

		// ArrayList<TalonFX> frontRightInstrument = new ArrayList<TalonFX>();
		// frontRightInstrument.add(frontRight);
		// frontRightOrchestra = new Orchestra(frontRightInstrument);

		// ArrayList<TalonFX> backLeftInstrument = new ArrayList<TalonFX>();
		// backLeftInstrument.add(backLeft);
		// backLeftOrchestra = new Orchestra(backLeftInstrument);

		// ArrayList<TalonFX> backRightInstrument = new ArrayList<TalonFX>();
		// backRightInstrument.add(backRight);
		// backRightOrchestra = new Orchestra(backRightInstrument);
	}

	/**
	 * Drives the robot at given x, y and rotational speeds. Speeds range from [-1, 1].
	 *
	 * @param xSpeed Speed of the robot in the x direction (forwards/backwards).
	 * @param ySpeed Speed of the robot in the y direction (sideways).
	 * @param rot Rotational rate of the robot.
	 */
	public void drive(double xSpeed, double ySpeed, double rot) {
		if (xSpeed < 0.05 && ySpeed < 0.05 && rot < 0.05 && orchestra.isPlaying())
		// if (xSpeed < 0.05 && ySpeed < 0.05 && rot < 0.05 && frontLeftOrchestra.isPlaying())
			return;
		if (doFieldOriented)
			mecanumDrive.driveCartesian(xSpeed * DrivetrainConst.MotorMaxSpeed, ySpeed * DrivetrainConst.MotorMaxSpeed, rot * DrivetrainConst.MotorMaxSpeed,
				navX.getRotation2d());
		else
			mecanumDrive.driveCartesian(xSpeed * DrivetrainConst.MotorMaxSpeed, ySpeed * DrivetrainConst.MotorMaxSpeed, rot * DrivetrainConst.MotorMaxSpeed);
	}

	public void setNeutralMode(NeutralMode neutralMode_p) { //brake/coast mode
		frontLeft.setNeutralMode(neutralMode_p);
		frontRight.setNeutralMode(neutralMode_p);
		backLeft.setNeutralMode(neutralMode_p);
		backRight.setNeutralMode(neutralMode_p);
		neutralMode = neutralMode_p;
	}
	public NeutralMode getNeutralMode() {
		return neutralMode;
	}

	public void setFieldOriented(boolean activateFieldOriented) {
		doFieldOriented = activateFieldOriented;
	}
	public boolean getFieldOriented() {
		return doFieldOriented;
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
		// if (frontLeftOrchestra.isPlaying()) {
		// 	frontLeftOrchestra.stop();
		// 	frontRightOrchestra.stop();
		// 	backLeftOrchestra.stop();
		// 	backRightOrchestra.stop();
		// }
		// frontLeftOrchestra.loadMusic(filepath);
		// frontRightOrchestra.loadMusic(filepath);
		// backLeftOrchestra.loadMusic(filepath);
		// backRightOrchestra.loadMusic(filepath);

		// frontLeftOrchestra.play();
		// frontRightOrchestra.play();
		// backLeftOrchestra.play();
		// backRightOrchestra.play();
	}
	public void playSong() {
		orchestra.play();
		// frontLeftOrchestra.play();
		// frontRightOrchestra.play();
		// backLeftOrchestra.play();
		// backRightOrchestra.play();
	}
	public void pauseSong() {
		orchestra.pause();
		// frontLeftOrchestra.pause();
		// frontRightOrchestra.pause();
		// backLeftOrchestra.pause();
		// backRightOrchestra.pause();
	}
	public void stopSong() {
		orchestra.stop();
		// frontLeftOrchestra.stop();
		// frontRightOrchestra.stop();
		// backLeftOrchestra.stop();
		// backRightOrchestra.stop();
	}
}
