package frc.robot.commands;

import java.util.function.BooleanSupplier;
import java.util.function.DoubleSupplier;

import edu.wpi.first.math.filter.SlewRateLimiter;
import edu.wpi.first.wpilibj2.command.CommandBase;
import frc.robot.Constants.DrivetrainConst;
import frc.robot.subsystems.Drivetrain;

public class DriveCartesian extends CommandBase {
    Drivetrain drivetrain;

    DoubleSupplier x;
    DoubleSupplier y;
    DoubleSupplier z;

    double xSquare;
    double ySquare;
    double zSquare;

    SlewRateLimiter xAccelerationCap = new SlewRateLimiter(3);
    SlewRateLimiter yAccelerationCap = new SlewRateLimiter(3);
    SlewRateLimiter zAccelerationCap = new SlewRateLimiter(3);

    BooleanSupplier povLeft; //whether the POV is pressed towards the left

    public DriveCartesian(Drivetrain drivetrain_p, 
        DoubleSupplier forward, DoubleSupplier right, DoubleSupplier rotation,
        BooleanSupplier povLeft_p)
    {
        drivetrain = drivetrain_p;

        x = forward;
        y = right;
        z = rotation;

        povLeft = povLeft_p;

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
    }

    @Override
    public void end(boolean interrupted) {
        drivetrain.drive(0, 0, 0);
    }

    private double closerToZero(double value1, double value2) {
        if (Math.min(Math.abs(value1), Math.abs(value2)) == Math.abs(value1))
            return value1;
        else
            return value2;
    }
}
