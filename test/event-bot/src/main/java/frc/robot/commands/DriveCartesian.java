package frc.robot.commands;

import java.util.function.BooleanSupplier;
import java.util.function.DoubleSupplier;
import java.util.function.IntSupplier;

import com.ctre.phoenix.motorcontrol.NeutralMode;

import edu.wpi.first.math.filter.SlewRateLimiter;
import edu.wpi.first.wpilibj2.command.CommandBase;
import frc.robot.Constants.DrivetrainConst;
import frc.robot.subsystems.Drivetrain;

public class DriveCartesian extends CommandBase {
    Drivetrain drivetrain;

    DoubleSupplier x;
    DoubleSupplier y;
    DoubleSupplier z;

    BooleanSupplier doBrake;
    boolean doBrakePrevState = false;
    BooleanSupplier doCoast;
    boolean doCoastPrevState = false;

    IntSupplier pov;

    double xSquare;
    double ySquare;
    double zSquare;

    SlewRateLimiter xAccelerationCap;
    SlewRateLimiter yAccelerationCap;
    SlewRateLimiter zAccelerationCap;

    public DriveCartesian(Drivetrain drivetrain_p, 
        DoubleSupplier forward, DoubleSupplier right, DoubleSupplier rotation,
        BooleanSupplier brakeButton, BooleanSupplier coastButton, 
        IntSupplier pov_p) 
    {
        drivetrain = drivetrain_p;

        x = forward;
        y = right;
        z = rotation;

        doBrake = brakeButton;
        doCoast = coastButton;

        pov = pov_p;

        addRequirements(drivetrain_p);
    }

    @Override
    public void execute() {
        xSquare = x.getAsDouble() * Math.abs(x.getAsDouble());
        ySquare = y.getAsDouble() * Math.abs(y.getAsDouble());
        zSquare = z.getAsDouble() * Math.abs(z.getAsDouble());

        if (Math.abs(x.getAsDouble()) > DrivetrainConst.JoystickDeadzone || Math.abs(y.getAsDouble()) > DrivetrainConst.JoystickDeadzone || Math.abs(z.getAsDouble()) > DrivetrainConst.JoystickDeadzone) {
            drivetrain.drive(closerToZero(xSquare, xAccelerationCap.calculate(xSquare)), 
                closerToZero(ySquare, yAccelerationCap.calculate(ySquare)), 
                closerToZero(zSquare, zAccelerationCap.calculate(zSquare)));
        }
        else
            drivetrain.drive(0, 0, 0);

        updateBrake();
    }

    @Override
    public void end(boolean interrupted) {
        drivetrain.drive(0, 0, 0);
    }

    private double closerToZero(double value1, double value2) {
        return Math.min(Math.abs(value1), Math.abs(value2));
    }

    private void updateBrake() {
        if (doBrake.getAsBoolean() && !doBrakePrevState) {
            drivetrain.setNeutralMode(NeutralMode.Brake);
            doBrakePrevState = true;
        }
        else if (!doBrake.getAsBoolean() && doBrakePrevState)
            doBrakePrevState = false;
        else if (doCoast.getAsBoolean() && !doCoastPrevState) {
            drivetrain.setNeutralMode(NeutralMode.Coast);
            doCoastPrevState = true;
        }
        else if (!doCoast.getAsBoolean() && doCoastPrevState) {
            doCoastPrevState = false;
        }
    }
}
