package frc.robot;

public class SwerveModuleConstants {
  public double steerKV;
  public double steerP;
  public double steerI;
  public double steerD;
  public double steerOffsetRotations;

  public double thrustKV;
  public double thrustP;
  public double thrustI;
  public double thrustD;
  public double thrustDistancePerTick;

  public int steerID;
  public int thrustID;

  public SwerveModuleConstants(
      double sk,
      double sp,
      double si,
      double sd,
      double so,
      double tk,
      double tp,
      double ti,
      double td,
      double tdpk,
      int sid,
      int tid) {
    steerKV = sk;
    steerP = sp;
    steerI = si;
    steerD = sd;
    steerOffsetRotations = so;
    thrustKV = tk;
    thrustP = tp;
    thrustI = ti;
    thrustD = td;
    thrustDistancePerTick = tdpk;

    steerID = sid;
    thrustID = tid;
  }
}
