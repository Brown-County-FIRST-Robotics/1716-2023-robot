package frc.robot;

import edu.wpi.first.apriltag.AprilTagFields;
import edu.wpi.first.math.estimator.SwerveDrivePoseEstimator;
import edu.wpi.first.math.geometry.Pose3d;
import edu.wpi.first.math.geometry.Rotation3d;
import edu.wpi.first.math.geometry.Transform3d;
import edu.wpi.first.math.geometry.Translation3d;
import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEvent;
import edu.wpi.first.networktables.NetworkTableInstance;
import java.io.IOException;
import java.util.EnumSet;
import java.util.Objects;
import java.util.Optional;

public class SSHelper {
  int numCams;
  SwerveDrivePoseEstimator poseEstimator;
  Transform3d[] camPos;
  NetworkTableInstance inst = NetworkTableInstance.getDefault();

  public SSHelper(SwerveDrivePoseEstimator est, int num_cams, Transform3d[] cam_pos) {
    poseEstimator = est;
    numCams = num_cams;
    camPos = cam_pos;
    try {
      AprilTagFields.kDefaultField.loadAprilTagLayoutField().getTagPose(0);
    } catch (IOException e) {
      throw new RuntimeException(e);
    }
    for (int i = 0; i < num_cams; i++) {
      int finalI = i;
      inst.addListener(
          inst.getTable("SecondSight_".concat(String.valueOf(i))).getEntry("offset"),
          EnumSet.of(NetworkTableEvent.Kind.kValueAll),
          (NetworkTableEvent ev) -> handleChange(finalI, ev));
    }
  }

  public void handleChange(int camnum, NetworkTableEvent ev) {
    NetworkTable table = inst.getTable("SecondSight_".concat(String.valueOf(camnum)));
    Pose3d camFieldPos;
    if (Objects.equals(table.getValue("proc").getString(), "Multiple")) {
      camFieldPos =
          new Pose3d(
              new Translation3d(
                  table.getValue("x").getDouble(),
                  table.getValue("y").getDouble(),
                  table.getValue("z").getDouble()),
              new Rotation3d(
                  table.getValue("roll").getDouble(),
                  table.getValue("pitch").getDouble(),
                  table.getValue("yaw").getDouble()));
    } else {
      try {
        Optional<Pose3d> april_pose =
            AprilTagFields.kDefaultField
                .loadAprilTagLayoutField()
                .getTagPose((int) table.getValue("tagid").getInteger());
        assert april_pose.isPresent();
        double yaw =
            april_pose
                .get()
                .toPose2d()
                .relativeTo(poseEstimator.getEstimatedPosition())
                .getRotation()
                .getRadians();
        Transform3d camTag =
            new Transform3d(
                new Translation3d(
                    table.getValue("dis").getDouble(),
                    table.getValue("lr").getDouble(),
                    table.getValue("ud").getDouble()),
                new Rotation3d(
                    table.getValue("roll").getDouble(), table.getValue("pitch").getDouble(), yaw));
        camFieldPos = april_pose.get().transformBy(camTag.inverse());
      } catch (IOException e) {
        throw new RuntimeException(e);
      }
    }
    Pose3d botToField = camFieldPos.transformBy(camPos[camnum].inverse());
    poseEstimator.addVisionMeasurement(
        botToField.toPose2d(),
        table.getEntry("offset").getLastChange() / 1000000.0
            - table.getEntry("offset").getDouble(0.0));
  }
}
