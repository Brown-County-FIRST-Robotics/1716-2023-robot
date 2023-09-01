package frc.robot.commands;

import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.button.CommandXboxController;
import frc.robot.subsystems.Drivetrain;

public class TeleopDrive extends CommandBase {
  Drivetrain drive;
  CommandXboxController controller;

  public TeleopDrive(Drivetrain drivetrain, CommandXboxController controller_) {
    drive = drivetrain;
    controller = controller_;
    addRequirements(drivetrain);
  }

  @Override
  public void execute() {
    drive.humanDrive(controller.getLeftY(), controller.getLeftX(), controller.getRightX());
  }
}
