# Apritag Module

The apriltag module contains code for finding apriltags and calculating the position of the apriltags. 

## Functions:

### `getPosition(image,camera_matrix, dist_coefficents, valid_tags=range(9), roll_threshold=0.3)`
`image` : The input image\
`camera_matrix` : The camera's calibration matrix\
`dist_coefficients` : The distortion coefficients of the camera\
`valid_tags` : (Default: 1-9) The apriltags to look for\
`roll_threshold` : (Default: 0.3 radians/17 degrees) The maximum roll of the apriltag, measured in radians. Helps remove false detections.

Returns: a list of Detection objects, or `None` if it fails. 

## Objects:

### `Detection(yaw, pitch, roll, left_right, up_down, distance, rms_error, tag_id)`

#### Public varibles:
`yaw`,`pitch`,`roll`,`left_right`,`up_down`,`distance` : The positions of the apriltag\
`rms_error` : The reprojection error of the apriltag, [read more about that here](https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga624af8a6641b9bdb487f63f694e8bb90)\
`tag_id` : The ID of the apriltag
