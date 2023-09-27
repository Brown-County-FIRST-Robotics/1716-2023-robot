package frc.robot;

public class SwerveModuleConstants {
  public double steerKV;
  public double steerP;
  public double steerI;
  public double steerD;
  public double steerOffset;
  public double steerRotationsPerAngle;

  public double thrustKV;
  public double thrustP;
  public double thrustI;
  public double thrustD;
  public double thrustDistancePerTick;

  public int steerID;
  public int thrustID;
  public int encoderID;

  public SwerveModuleConstants(
      double sk,
      double sp,
      double si,
      double sd,
      double so,
      double sr,
      double tk,
      double tp,
      double ti,
      double td,
      double tdpk,
      int sid,
      int tid,
      int eid) {
    steerKV = sk;
    steerP = sp;
    steerI = si;
    steerD = sd;
    steerOffset = so;
    steerRotationsPerAngle = sr;
    thrustKV = tk;
    thrustP = tp;
    thrustI = ti;
    thrustD = td;
    thrustDistancePerTick = tdpk;

    steerID = sid;
    thrustID = tid;
    encoderID = eid;
  }

  public double getRotationsFromAngle(double angRads) {
    return (angRads - steerOffset) * steerRotationsPerAngle;
  }

  public double getAngleFromRotations(double rotations) {
    return (rotations / steerRotationsPerAngle) + steerOffset;
  }
}
