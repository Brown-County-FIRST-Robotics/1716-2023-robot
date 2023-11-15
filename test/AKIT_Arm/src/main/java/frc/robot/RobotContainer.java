package frc.robot;

import edu.wpi.first.wpilibj2.command.Commands;
import edu.wpi.first.wpilibj2.command.button.CommandXboxController;
import frc.robot.subsystems.arm.Arm;
import frc.robot.subsystems.arm.ArmSparkMaxIO;

public class RobotContainer {
  private final CommandXboxController controller = new CommandXboxController(0);
  private final Arm arm;

  public RobotContainer() {
    arm = new Arm(new ArmSparkMaxIO());
    configButtonBindings();
  }

  public void configButtonBindings() {
    controller
        .a()
        .whileFalse(Commands.run(() -> arm.addToGoal(-controller.getLeftY()), arm))
        .whileTrue(Commands.run(() -> arm.setGoal(2), arm));
  }
}
