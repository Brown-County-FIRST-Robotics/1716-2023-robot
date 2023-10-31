package frc.robot;

import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.kinematics.SwerveModuleState;
import edu.wpi.first.networktables.GenericEntry;
import edu.wpi.first.networktables.NetworkTableEntry;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.CommandScheduler;
import frc.robot.subsystems.SwerveModule;

/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {

  SwerveModule mod;
  GenericEntry off;
    GenericEntry vel;

      GenericEntry inpPos;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    double THRUST_DISTANCE_PER_TICK = .0254 * 4.0 * Math.PI / (2048.0 * 6.75);
    double THRUST_KV = 1023.0 * 600.0 / (6380.0 * 2048.0);
    double STEER_KV = 1.0/300.0;
    mod=new SwerveModule(new SwerveModuleConstants(
            STEER_KV, 0, 0, 0, 0.067, THRUST_KV, 0, 0, 0, THRUST_DISTANCE_PER_TICK, 31, 1));
    // Instantiate our RobotContainer.  This will perform all our button bindings, and put our
    // autonomous chooser on the dashboard.


    off=Shuffleboard.getTab("TAASAS").add("Offset", mod.constants.steerOffsetRotations).getEntry();
    vel=Shuffleboard.getTab("TAASAS").add("vel", 0.0).getEntry();

    inpPos=Shuffleboard.getTab("TAASAS").add("inppos", 0.0).getEntry();


  }

  /**
   * This function is called every robot packet, no matter the mode. Use this for items like
   * diagnostics that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and
   * SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {
    // Runs the Scheduler.  This is responsible for polling buttons, adding newly-scheduled
    // commands, running already-scheduled commands, removing finished or interrupted commands,
    // and running subsystem periodic() methods.  This must be called from the robot's periodic
    // block in order for anything in the Command-based framework to work.

    mod.constants.steerOffsetRotations=off.getDouble(mod.constants.steerOffsetRotations);
    //SmartDashboard.putNumber("Pos",mod.constants.steerOffsetRotations+mod.getModulePosition().angle.getRotations());

  }

  /** This function is called once each time the robot enters Disabled mode. */
  @Override
  public void disabledInit() {}

  @Override
  public void disabledPeriodic() {}

  /** This autonomous runs the autonomous command selected by your {@link RobotContainer} class. */
  @Override
  public void autonomousInit() {

  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {}

  @Override
  public void teleopInit() {

    // This makes sure that the autonomous stops running when
    // teleop starts running. If you want the autonomous to
    // continue until interrupted by another command, remove
  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
        double v=vel.getFloat((float) -0.87);
    double r=inpPos.getFloat(0);
    mod.setModuleState(new SwerveModuleState(v,Rotation2d.fromRotations(r)));

  }

  @Override
  public void testInit() {
    // Cancels all running commands at the start of test mode.
  }

  /** This function is called periodically during test mode. */
  @Override
  public void testPeriodic() {}

  /** This function is called once when the robot is first started up. */
  @Override
  public void simulationInit() {}

  /** This function is called periodically whilst in simulation. */
  @Override
  public void simulationPeriodic() {}
}
