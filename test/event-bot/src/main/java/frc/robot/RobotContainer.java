package frc.robot;

import edu.wpi.first.wpilibj.XboxController;

import com.ctre.phoenix.motorcontrol.NeutralMode;

import edu.wpi.first.wpilibj2.command.InstantCommand;
import edu.wpi.first.wpilibj2.command.button.CommandXboxController;
import frc.robot.Constants.IOConstants;
import frc.robot.commands.DriveCartesian;
import frc.robot.subsystems.Drivetrain;

public class RobotContainer {
	private final Drivetrain drivetrain = new Drivetrain();

	XboxController controller = new XboxController(IOConstants.DriverControllerPort);
	CommandXboxController triggerController = new CommandXboxController(IOConstants.DriverControllerPort);
	
	String[] songs = { "MrRoboto.chrp", "Megalovania.chrp", "RickRoll.chrp", "GandalfSaxGuy.chrp" };

	public RobotContainer() {
		configureButtonBindings();

		drivetrain.setDefaultCommand(new DriveCartesian(drivetrain,
			() -> { return -controller.getLeftY(); },
			() -> { return controller.getLeftX(); },
			() -> { return controller.getRightX(); },
			() -> { return controller.getPOV() == 270; }));
	}

	private void configureButtonBindings() {
		//down on D-pad, controls drivetrain settings
		triggerController.pov(180).and(triggerController.a())
			.onTrue(new InstantCommand(() -> { drivetrain.toggleFieldOriented(); }, drivetrain));

		triggerController.pov(180).and(triggerController.b())
			.onTrue(new InstantCommand(() -> { drivetrain.toggleNeutralMode(); }, drivetrain));

		triggerController.pov(180).and(triggerController.y())
			.onTrue(new InstantCommand(() -> { drivetrain.resetGyroscope(); }, drivetrain));

		//left on D-pad, controls music playing
		triggerController.pov(270).and(triggerController.a())
			.onTrue(new InstantCommand(() -> { drivetrain.playSong(songs[0]); System.out.println("playing song 0"); }));
		triggerController.pov(270).and(triggerController.b())
			.onTrue(new InstantCommand(() -> { drivetrain.playSong(songs[1]); System.out.println("playing song 1"); }));
		triggerController.pov(270).and(triggerController.x())
			.onTrue(new InstantCommand(() -> { drivetrain.playSong(songs[2]); System.out.println("playing song 2");}));
		triggerController.pov(270).and(triggerController.y())
			.onTrue(new InstantCommand(() -> { drivetrain.playSong(songs[3]); System.out.println("playing song 3"); }));
		
		//Change max speed
		triggerController.back()
			.and(triggerController.start()
			.and(triggerController.leftBumper()))
			.onTrue(new InstantCommand(() -> { drivetrain.decrementMaxSpeed(); }, drivetrain));
		triggerController.back()
			.and(triggerController.start()
			.and(triggerController.rightBumper()))
			.onTrue(new InstantCommand(() -> { drivetrain.incrementMaxSpeed(); }, drivetrain));
	}
}
