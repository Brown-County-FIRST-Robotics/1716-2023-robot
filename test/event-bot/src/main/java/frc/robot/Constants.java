package frc.robot;

public final class Constants {
	public static final class DriveConstants {
		public static final int FrontLeftMotorPort = 0;
		public static final int RearLeftMotorPort = 1;
		public static final int FrontRightMotorPort = 2;
		public static final int RearRightMotorPort = 3;

		public static final int[] FrontLeftEncoderPorts = new int[] {0, 1};
		public static final int[] RearLeftEncoderPorts = new int[] {2, 3};
		public static final int[] FrontRightEncoderPorts = new int[] {4, 5};
		public static final int[] RearRightEncoderPorts = new int[] {6, 7};

		public static final boolean FrontLeftEncoderReversed = false;
		public static final boolean RearLeftEncoderReversed = true;
		public static final boolean FrontRightEncoderReversed = false;
		public static final boolean RearRightEncoderReversed = true;

		public static final double MotorMaxSpeed = 0.4;
	}

	public static final class IOConstants {
		public static final int DriverControllerPort = 0;
	}
}
