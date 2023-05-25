package frc.robot;

import edu.wpi.first.wpilibj.XboxController;
import frc.robot.Constants.IOConstants;
import frc.robot.subsystems.DriveSubsystem;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.RunCommand;

public class RobotContainer {
	private final DriveSubsystem robotDrive = new DriveSubsystem();

	XboxController controller = new XboxController(IOConstants.DriverControllerPort);

	public RobotContainer() {
		configureButtonBindings();

		robotDrive.setDefaultCommand(
				new RunCommand(() -> robotDrive.drive(
					-controller.getLeftY(),
					-controller.getLeftX(),
					-controller.getRightX()),
					robotDrive));
	}

	private void configureButtonBindings() {}

	public Command getAutonomousCommand() {
		return null;
	}
}
