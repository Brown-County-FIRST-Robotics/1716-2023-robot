'''
TODO: add detection class
TODO: make getPosition return

'''

import math, time, sys, json
import apriltag
import cv2
import numpy as np

#cam = cv2.VideoCapture()
#cam.open(f'/dev/video{int(sys.argv[1])}')

tag_size = 8 / 2.54

#video_size = (cam.get(cv2.CAP_PROP_FRAME_WIDTH), cam.get(cv2.CAP_PROP_FRAME_HEIGHT))
roll_threshold = 0.3

#with open(sys.argv[2], 'r') as f:
#    mtx, dist = json.loads(f.read())
#    mtx = np.array(mtx)
#    dist = np.array(dist)

#with open('camera_error.json', 'r') as f:
#    camera_error = json.loads(f.read())


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

    imgPointsArr = []
    objPointsArr = []
    opointsArr = []

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

            imgPointsArr.append(imagePoints)
            objPointsArr.append(object_pts)
            opointsArr.append(opoints)

            # mtx - the camera calibration's intrinsics
            _, prvecs, ptvecs = cv2.solvePnP(object_pts, imagePoints, camera_mtx, dist_coefficients, flags=cv2.SOLVEPNP_ITERATIVE)

            # Draws the edges of the pose onto the image
            pitch = prvecs[0]
            yaw = prvecs[1]
            roll = prvecs[2]

            left_right = -ptvecs[0] - ptvecs[2] / 4
            up_down = (ptvecs[1] + ptvecs[2] / 16) * 2
            distance = ptvecs[2]

            if math.fabs(roll) > roll_threshold:
                print('discarded a value')
                continue
            return pitch, yaw, roll, left_right, up_down, distance
    else:
        return None
