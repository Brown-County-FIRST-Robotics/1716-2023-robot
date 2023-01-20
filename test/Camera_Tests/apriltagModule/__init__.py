'''
TODO: add detection class
TODO: make getPosition return

'''

import math, time, sys, json
import apriltag
import cv2
import numpy as np

tag_size = 8 / 2.54

roll_threshold = 0.3


class Detection:
    def __init__(self, yaw, pitch, roll, left_right, up_down, distance, RMSError, tagID):
        self.yaw = yaw
        self.pitch = pitch
        self.roll = roll
        self.left_right = left_right
        self.up_down = up_down
        self.distance = distance
        self.RMSError = RMSError
        self.tagID = tagID


def convert(pt):
    return float(pt[0]), float(pt[1])


def getPosition(img, camera_mtx, dist_coefficients):
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

    detector = apriltag.Detector(options)

    detection_results = detector.detect(gray)

    num_detections = len(detection_results)

    img_points_arr = []
    obj_points_arr = []
    points_array = []
    detections=[]
    if num_detections > 0:
        for i, detection in enumerate(detection_results):
            if detection.tag_id > 8:
                continue

            imagePoints = detection.corners.reshape(1, 4, 2)

            # Get the tag size from .json file

            ob_pt1 = [-tag_size / 2, -tag_size / 2, 0.0]
            ob_pt2 = [tag_size / 2, -tag_size / 2, 0.0]
            ob_pt3 = [tag_size / 2, tag_size / 2, 0.0]
            ob_pt4 = [-tag_size / 2, tag_size / 2, 0.0]
            ob_pts = ob_pt1 + ob_pt2 + ob_pt3 + ob_pt4
            object_pts = np.array(ob_pts).reshape(4, 3)

            opoints = np.array([
                -1, -1, 0,
                1, -1, 0,
                1, 1, 0,
                -1, 1, 0,
                -1, -1, -2 * 1,
                1, -1, -2 * 1,
                1, 1, -2 * 1,
                -1, 1, -2 * 1,
            ]).reshape(-1, 1, 3) * 0.5 * tag_size

            img_points_arr.append(imagePoints)
            obj_points_arr.append(object_pts)
            points_array.append(opoints)

            # mtx - the camera calibration's intrinsics
            good, prvecs, ptvecs, RMS = cv2.solvePnPGeneric(object_pts, imagePoints, camera_mtx, dist_coefficients,
                                                flags=cv2.SOLVEPNP_ITERATIVE)
            assert good, 'something went wrong with solvePnP'
            # Draws the edges of the pose onto the image
            pitch = prvecs[0][0]
            yaw = prvecs[0][1]
            roll = prvecs[0][2]

            left_right = -ptvecs[0][0] - ptvecs[0][2] / 4
            up_down = (ptvecs[0][1] + ptvecs[0][2] / 16) * 2
            distance = ptvecs[0][2]

            if math.fabs(roll) > roll_threshold:
                print('discarded a value')
                continue
            detections.append(Detection(yaw, pitch, roll, left_right, up_down, distance, RMS,
                             detection.tag_id))
    else:
        return None
    return detections
