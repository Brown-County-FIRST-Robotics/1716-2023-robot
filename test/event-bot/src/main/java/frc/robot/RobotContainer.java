package frc.robot;

import edu.wpi.first.wpilibj.XboxController;
import static edu.wpi.first.wpilibj.XboxController.Button;

import com.ctre.phoenix.motorcontrol.NeutralMode;

import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.InstantCommand;
import edu.wpi.first.wpilibj2.command.button.CommandXboxController;
import edu.wpi.first.wpilibj2.command.button.JoystickButton;
import frc.robot.Constants.IOConstants;
import frc.robot.commands.DriveCartesian;
import frc.robot.subsystems.Drivetrain;

public class RobotContainer {
	private final Drivetrain drivetrain = new Drivetrain();

	XboxController controller = new XboxController(IOConstants.DriverControllerPort);
	CommandXboxController triggerController = new CommandXboxController(IOConstants.DriverControllerPort);
	
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
			.onTrue(new InstantCommand(() -> { drivetrain.setFieldOriented(!drivetrain.getFieldOriented()); }, drivetrain));

		triggerController.pov(180).and(triggerController.b())
			.onTrue(new InstantCommand(() -> { 
				if (drivetrain.getNeutralMode() == NeutralMode.Coast) {
					drivetrain.setNeutralMode(NeutralMode.Brake);
				}
				else if (drivetrain.getNeutralMode() == NeutralMode.Brake) {
					drivetrain.setNeutralMode(NeutralMode.Coast);
				}
			}, drivetrain));

		triggerController.pov(180).and(triggerController.y())
			.onTrue(new InstantCommand(() -> { drivetrain.resetGyroscope(); }, drivetrain));
	}
}
