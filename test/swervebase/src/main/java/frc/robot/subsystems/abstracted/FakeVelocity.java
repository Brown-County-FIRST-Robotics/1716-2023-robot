package frc.robot.subsystems.abstracted;

import edu.wpi.first.wpilibj.Timer;

public class FakeVelocity implements VelocityMotor{
    Timer tm=new Timer();
    double pos=0;
    double lastVel=0;
    public FakeVelocity() {
    }

    @Override
    public void setVelocity(double vel) {
        pos+=tm.get()*lastVel;
        lastVel=vel;
        tm.reset();
        tm.start();
    }

    @Override
    public double getPos() {
        return pos;
    }
}
