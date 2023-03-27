from rr1716 import ColorRange

xy_pid_factor = (0.015, 0.001, 0)  # TODO: change
r_pid_factor = (0.005,0,0)  # TODO: change

r_error_threshold = 5  # TODO: change

drive_error_threshold = 3  # TODO: change

accel_angle_threshold = 10  # TODO: change

TARGET_CUBE_SIZE = 100

cube_color_range = ColorRange.ColRange(lower=[ -5, -100, -100 ], upper=[ 5, 100, 100 ], lower_ratio=3.0/5.0, upper_ratio=5.0/3.0)
