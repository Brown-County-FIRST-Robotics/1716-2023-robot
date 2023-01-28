import math
import apriltag
import cv2
import numpy as np

tag_size = 8 / 2.54


class Detection:
    def __init__(self, yaw, pitch, roll, left_right, up_down, distance, rms_error, tag_id):
        self.yaw = yaw
        self.pitch = pitch
        self.roll = roll
        self.left_right = left_right
        self.up_down = up_down
        self.distance = distance
        self.RMSError = rms_error
        self.tagID = tag_id

        self.yaw_std = -1
        self.pitch_std = -1
        self.left_right_std = -1
        self.up_down_std = -1
        self.distance_std = -1
        self.error = -1

    def calcError(self, error_matrix=None, error_threshold=30):  # TODO: add error threshold
        if error_matrix is None:
            error_matrix = np.array([  # TODO: add real values
                [1, 1, 1, 1, 1, 1, 1],
                [1, 1, 1, 1, 1, 1, 1],
                [1, 1, 1, 1, 1, 1, 1],
                [1, 1, 1, 1, 1, 1, 1],
                [1, 1, 1, 1, 1, 1, 1],
                [1, 1, 1, 1, 1, 1, 1],
            ])
        input_matrix = np.array([  # TODO: rename
            self.yaw,
            self.pitch,
            self.roll,
            self.left_right,
            self.up_down,
            self.distance,
            self.RMSError
        ])
        self.yaw_std, self.pitch_std, self.left_right_std, self.up_down_std, self.distance_std, self.error = [i[0] for i in np.dot(error_matrix, input_matrix).tolist()]
        if self.error > error_threshold:
            print(f'discarded a value (error:{self.error})')


def convert(pt):
    return float(pt[0]), float(pt[1])


def getPosition(img, camera_matrix, dist_coefficients, valid_tags=range(1, 9), roll_threshold=20):
    """
    This function takes an image and returns the position of apriltags in the image

    :param img: The image you want to find apriltags in
    :param camera_matrix: The camera's calibration matrix
    :param dist_coefficients: The distortion coefficients of the camera
    :param valid_tags: (Default: 1-9) The apriltags to look for
    :param roll_threshold: (Default: 0.3 radians/17 degrees) The maximum roll of the apriltag. Helps remove false detections.
    :return: A list of Detection objects, or None if it fails
    :rtype: list(Detection objects)
    """
    # Convert Images to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # AprilTag detector options
    options = apriltag.DetectorOptions(families='tag16h5',
                                       border=1,
                                       nthreads=4,
                                       quad_decimate=1.0,
                                       quad_blur=0.0,
                                       refine_edges=True,
                                       refine_decode=False,
                                       refine_pose=True,
                                       debug=False,
                                       quad_contours=True)
    # Create a detector with given options
    detector = apriltag.Detector(options)
    # Find the apriltags
    detection_results = detector.detect(gray)

    detections = []
    if len(detection_results) > 0:  # Check if there are any apriltags
        for detection in detection_results:
            # Check if apriltag is allowed
            if detection.tag/home/colin/Documents/1716-2023-robot/docs/ApritagModule.md_id not in valid_tags:
                continue

            image_points = detection.corners.reshape(1, 4, 2)

            ob_pt1 = [-tag_size / 2, -tag_size / 2, 0.0]
            ob_pt2 = [tag_size / 2, -tag_size / 2, 0.0]
            ob_pt3 = [tag_size / 2, tag_size / 2, 0.0]
            ob_pt4 = [-tag_size / 2, tag_size / 2, 0.0]
            ob_pts = ob_pt1 + ob_pt2 + ob_pt3 + ob_pt4
            object_pts = np.array(ob_pts).reshape(4, 3)

            # Solve for rotation and translation
            good, rotation_vector, translation_vector, rms = cv2.solvePnPGeneric(object_pts, image_points, camera_matrix,
                                                                                 dist_coefficients,
                                                                                 flags=cv2.SOLVEPNP_ITERATIVE)
            assert good, 'something went wrong with solvePnP'

            # Map rotation_vector
            pitch, yaw, roll = rotation_vector[0]*180/math.pi

            left_right = (-translation_vector[0][0] - translation_vector[0][2] / 4)*2.54
            up_down = ((translation_vector[0][1] + translation_vector[0][2] / 16) * 2)*2.54
            distance = translation_vector[0][2]*2.54

            # Check if roll is within limit
            if math.fabs(roll) > roll_threshold:
                print('discarded a value (roll)')
                continue
            detections.append(Detection(yaw, pitch, roll, left_right, up_down, distance, rms,
                                        detection.tag_id))
    else:
        # If there are no apriltags, return None
        return None
    return detections
