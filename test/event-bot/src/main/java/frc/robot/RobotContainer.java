package frc.robot;

import edu.wpi.first.wpilibj.XboxController;
import frc.robot.Constants.IOConstants;
import frc.robot.commands.DriveCartesian;
import frc.robot.subsystems.Drivetrain;

public class RobotContainer {
	private final Drivetrain drivetrain = new Drivetrain();

	XboxController controller = new XboxController(IOConstants.DriverControllerPort);

	public RobotContainer() {
		configureButtonBindings();

		drivetrain.setDefaultCommand(new DriveCartesian(drivetrain,
			() -> { return -controller.getLeftY(); },
			() -> { return controller.getLeftX(); },
			() -> { return controller.getRightX(); },
			() -> { return controller.getBButton(); },
			() -> { return controller.getAButton(); },
			() -> { return controller.getPOV(); }));
	}

	private void configureButtonBindings() {}
}
