// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.PneumaticHub;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.Commands;
import edu.wpi.first.wpilibj2.command.InstantCommand;
import edu.wpi.first.wpilibj2.command.button.CommandXboxController;
import frc.robot.commands.TeleopDrive;
import frc.robot.subsystems.Drivetrain;

public class RobotContainer {
  CommandXboxController controller = new CommandXboxController(0);
  PneumaticHub hub = new PneumaticHub(Constants.IO.HUB_ID);
  Drivetrain drivetrain = new Drivetrain(hub);
  TeleopDrive teleopDrive = new TeleopDrive(drivetrain, controller);

  public RobotContainer() {
    configureBindings();
    setDefaultCommands();
  }

  private void setDefaultCommands() {
    drivetrain.setDefaultCommand(teleopDrive);
  }

  private void configureBindings() {
    controller
        .leftTrigger(0.2)
        .onTrue(
            new InstantCommand(
                () -> {
                  drivetrain.toggleSolenoidPos();
                  // TODO: add LEDs
                },
                drivetrain));
  }

  public Command getAutonomousCommand() {
    return Commands.print("No autonomous command configured");
  }
}
