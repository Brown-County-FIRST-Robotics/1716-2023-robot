package frc.robot;

import edu.wpi.first.math.geometry.Translation2d;
import edu.wpi.first.wpilibj.DriverStation;
import edu.wpi.first.wpilibj.GenericHID;
import edu.wpi.first.wpilibj.XboxController;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.InstantCommand;
import edu.wpi.first.wpilibj2.command.button.CommandXboxController;
import frc.robot.commands.TeleopDrive;
import frc.robot.subsystems.Bucket;
import frc.robot.subsystems.Drivetrain;

/**
 * This class is where the bulk of the robot should be declared. Since Command-based is a
 * "declarative" paradigm, very little robot logic should actually be handled in the {@link Robot}
 * periodic methods (other than the scheduler calls). Instead, the structure of the robot (including
 * subsystems, commands, and button mappings) should be declared here.
 */
public class RobotContainer {
  /** The container for the robot. Contains subsystems, OI devices, and commands. */
  private final SendableChooser<Command> autoChooser = new SendableChooser<Command>();

  private final Drivetrain drivetrain = new Drivetrain();
  private final Bucket bucket = new Bucket();

  private final CommandXboxController controller =
      new CommandXboxController(Constants.IO.CONTROLLER1);

  public RobotContainer() {
    autoChooser.addOption(
        "Leave Community", drivetrain.makePositionCommand(new Translation2d(5, 0)));
    Shuffleboard.getTab("Pre match").add("Auton choser", autoChooser);
    // Configure the button bindings
    configureButtonBindings();

    // Set Default Commands
    setDefaultCommands();

    // Setup shuffleboard "Main Dashboard"

  }

  // Set Subsystem Default Commands
  public void setDefaultCommands() {
    drivetrain.setDefaultCommand(new TeleopDrive(drivetrain, controller));
  }

  /**
   * Use this method to define your button->command mappings. Buttons can be created by
   * instantiating a {@link GenericHID} or one of its subclasses ({@link
   * edu.wpi.first.wpilibj.Joystick} or {@link XboxController}), and then passing it to a {@link
   * edu.wpi.first.wpilibj2.command.button.JoystickButton}.
   */
  private void configureButtonBindings() {
    controller
        .leftBumper()
        .or(controller.rightBumper())
        .onTrue(
            new InstantCommand(
                () -> {
                  bucket.SetPosition(false);
                }));

    controller
        .leftTrigger()
        .or(controller.rightTrigger())
        .onTrue(
            new InstantCommand(
                () -> {
                  bucket.SetPosition(true);
                }));
  }

  /**
   * Use this to pass the autonomous command to the main {@link Robot} class.
   *
   * @return the command to run in autonomous
   */
  public Command getAutonomousCommand() {
    drivetrain.setPos(
        DriverStation.getAlliance() == DriverStation.Alliance.Red
            ? Constants.RED_INIT_POSE
            : Constants.BLUE_INIT_POSE);
    return autoChooser.getSelected();
  }
}
