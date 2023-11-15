package frc.robot.subsystems.arm;

import edu.wpi.first.wpilibj2.command.SubsystemBase;
import org.littletonrobotics.junction.Logger;
import org.littletonrobotics.junction.networktables.LoggedDashboardInput;
import org.littletonrobotics.junction.networktables.LoggedDashboardNumber;

public class Arm extends SubsystemBase {
  private final ArmIO io;
  private final ArmIOInputsAutoLogged inputs = new ArmIOInputsAutoLogged();
  private boolean touchingIn = false;
  private boolean touchingOut = false;
  private final LoggedDashboardNumber arbsetter=new LoggedDashboardNumber("ArbFF");
  private double goal=0;

  public Arm(ArmIO io) {
    this.io = io;
  }

  @Override
  public void periodic() {
    io.updateInputs(inputs);
    Logger.getInstance().processInputs("Arm", inputs);
    Logger.getInstance().recordOutput("Arbs", 133214);
    System.out.println(inputs.inSwitch);
    System.out.println(inputs.outSwitch);
    io.setArbff(arbsetter.get());

    if (!touchingIn && inputs.inSwitch) {
      io.setEncPos(0);
      io.setGoalPos(0);
      touchingIn = true;
    }
    if (!touchingOut && inputs.outSwitch) {


      io.setEncPos(290.0 * Math.PI / 360.0);
      io.setGoalPos(290*Math.PI/360.0);
      touchingOut = true;
    }
    if (!inputs.inSwitch) {
      touchingIn = false;
    }
    if (!inputs.outSwitch) {
      touchingOut = false;
    }
  }

  public void addToGoal(double added) {
    io.setGoalPos(inputs.posRads + added);
    System.out.println(inputs.posRads);
  }

  public void setGoal(double pos) {
    io.setGoalPos(pos);
  }
}
