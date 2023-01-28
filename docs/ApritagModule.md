# Apritag Module

The apriltag module contains code for finding apriltags and calculating the position of the apriltags. 

## Functions:

### `getPosition(image,camera_matrix, dist_coefficents, valid_tags=range(1, 9), roll_threshold=20)`
`image` : The input image\
`camera_matrix` : The camera's calibration matrix\
`dist_coefficients` : The distortion coefficients of the camera\
`valid_tags` : (Default: 1-9) The apriltags to look for\
`roll_threshold` : (Default: 20 degrees) The maximum roll of the apriltag, measured in degrees. Helps remove false detections.

Returns: a list of Detection objects, or `None` if it fails. 

## Objects:

### `Detection(yaw, pitch, roll, left_right, up_down, distance, rms_error, tag_id)`

#### Public variables:
`yaw`,`pitch`,`roll`,`left_right`,`up_down`,`distance` : The positions of the apriltag\
`rms_error` : The reprojection error of the apriltag, [read more about that here](https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga624af8a6641b9bdb487f63f694e8bb90)\
`tag_id` : The ID of the apriltag
`yaw_std`,`pitch_std`,`roll_std`,`left_right_std`,`up_down_std`,`distance_std` : The standard deviation of the position of the apriltag\
`error` : A general error factor, used to remove false detections

#### Methods:
#### `calcError(error_matrix=None, error_threshold=30)`
`error_matrix` : The matrix used for error calculations. Uses default matrix unless set. \
`error_threshold` : The highest possible error value it will accept. 
