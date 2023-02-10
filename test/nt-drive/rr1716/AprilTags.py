import math
import apriltag
import cv2
import numpy as np
from rr1716.positions import *

tag_size = 8 / 2.54


class Detection:
    def __init__(self, yaw, left_right, distance, rms_error, tag_id):
        self.yaw = yaw
        self.left_right = left_right
        self.distance = distance
        self.RMSError = rms_error
        self.tagID = tag_id

        self.field_yaw = -1
        self.field_x = -1
        self.field_y = -1

        self.yaw_std = -1
        self.left_right_std = -1
        self.distance_std = -1
        self.error = -1

    def calcError(self, error_matrix=None, error_threshold=3000):  # TODO: add error threshold
        if error_matrix is None:
            error_matrix = np.array([  # TODO: add real values
                [1, 1, 1, 1],
                [1, 1, 1, 1],
                [1, 1, 1, 1],
                [1, 1, 1, 1],
            ])
        input_matrix = np.array([  # TODO: rename
            self.yaw,
            self.left_right,
            self.distance,
            self.RMSError
        ])
        self.yaw_std, self.left_right_std, self.distance_std, self.error = [i[0] for i in np.dot(error_matrix, input_matrix).tolist()]
        if self.error > error_threshold:
            print(f'discarded a value (error:{self.error})')

    def calcFieldPos(self):
        pos = apriltagPositions[self.tagID]
        camera_theta = 180 + self.yaw + pos[3]
        thetaCA = camera_theta - math.atan(self.left_right / self.distance) * 180 / math.pi
        camera_Y = pos[1] - (math.sqrt(self.left_right ** 2 + self.distance ** 2) * math.sin(thetaCA * math.pi / 180))
        camera_X = pos[0] - (math.sqrt(self.left_right ** 2 + self.distance ** 2) * math.cos(thetaCA * math.pi / 180))
        self.field_yaw = thetaCA
        self.field_x = camera_X
        self.field_y = camera_Y


def getPosition(img, camera_matrix, dist_coefficients, valid_tags=range(1, 9), roll_threshold=20, check_hamming=True):
    """
    This function takes an image and returns the position of apriltags in the image

    :param img: The image you want to find apriltags in (must be grayscale)
    :param camera_matrix: The camera's calibration matrix
    :param dist_coefficients: The distortion coefficients of the camera
    :param valid_tags: (Default: 1-9) The apriltags to look for
    :param roll_threshold: (Default: 0.3 radians/17 degrees) The maximum roll of the apriltag. Helps remove false detections.
    :param check_hamming: (Default: True) Checks if the hamming value is 0
    :return: A list of Detection objects, or None if it fails
    :rtype: list(Detection objects)
    """
    # Check if image is grayscale
    assert len(img.shape)==2, 'Image must be grayscale'

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
    detection_results = detector.detect(img)

    detections = []
    if len(detection_results) > 0:  # Check if there are any apriltags
        for detection in detection_results:
            # Check if apriltag is allowed
            if detection.tag_id not in valid_tags:
                continue
            if detection.hamming != 0 and check_hamming:
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
                print(f'discarded a value (roll:{roll})')
                continue
            detections.append(Detection(yaw, left_right, distance, rms,
                                        detection.tag_id))
    else:
        # If there are no apriltags, return None
        return None
    return detections


if __name__ == "main":
    pass
else:
    pass