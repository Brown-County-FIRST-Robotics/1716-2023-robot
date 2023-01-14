# make networktables

import apriltag
import cv2
import numpy as np
import sys, os
from networktables import NetworkTables


# I am getting the tag sizes from a json file for all apriltags

# tag_sizes, tvecs, rvecs = get_extrinsics(json_file)

cam = cv2.VideoCapture()
cam.open('/dev/video0')

tag_size = 8

video_size = (cam.get(cv2.CAP_PROP_FRAME_WIDTH), cam.get(cv2.CAP_PROP_FRAME_HEIGHT))

print(video_size)

focal_length = video_size[1]
camera_center = (video_size[1] / 2, video_size[0] / 2)


mtx = np.array(
            [[focal_length, 0, camera_center[0]],
             [0, focal_length, camera_center[1]],
             [0, 0, 1]], dtype="double")
def convert(pt):
    return (int(pt[0]), int(pt[1]))


NetworkTables.initialize(server="10.17.16.2")

table = NetworkTables.getTable("apriltag")
table.putNumber("up_down", 0)
table.putNumber("left_right", 0)
table.putNumber("distance", 0)
table.putNumber("roll", 0)
table.putNumber("pitch", 0)
table.putNumber("yaw", 0)




while True:
    img = cam.read()[1]
    cv2.imshow("Image", img)

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

    # Detect the apriltags in the image
    detection_results = detector.detect(gray)

    # Amount of april tags detected
    num_detections = len(detection_results)

    imgPointsArr = []
    objPointsArr = []
    opointsArr = []

    if num_detections > 0:
        for i, detection in enumerate(detection_results):
            if detection.tag_id!=3:
                continue
            (ptA, ptB, ptC, ptD) = detection.corners
            ptA = convert(ptA)
            ptB = convert(ptB)
            ptC = convert(ptC)
            ptD = convert(ptD)

            #cv2.line(img, ptA, ptB, (0, 0, 255), 2)
            #/cv2.line(img, ptB, ptC, (0, 0, 255), 2)
            #cv2.line(img, ptC, ptD, (0, 0, 255), 2)
            #cv2.line(img, ptD, ptA, (0, 0, 255), 2)
            cv2.imshow("Image", img)

            if mtx is not None:
                imagePoints = detection.corners.reshape(1,4,2)

                # Get the tag size from .json file

                ob_pt1 = [-tag_size/2, -tag_size/2, 0.0]
                ob_pt2 = [ tag_size/2, -tag_size/2, 0.0]
                ob_pt3 = [ tag_size/2,  tag_size/2, 0.0]
                ob_pt4 = [-tag_size/2,  tag_size/2, 0.0]
                ob_pts = ob_pt1 + ob_pt2 + ob_pt3 + ob_pt4
                object_pts = np.array(ob_pts).reshape(4,3)

                opoints = np.array([
                    -1, -1, 0,
                    1, -1, 0,
                    1,  1, 0,
                    -1,  1, 0,
                    -1, -1, -2*1,
                    1, -1, -2*1,
                    1,  1, -2*1,
                    -1,  1, -2*1,
                ]).reshape(-1, 1, 3) * 0.5*tag_size


                imgPointsArr.append(imagePoints)
                objPointsArr.append(object_pts)
                opointsArr.append(opoints)

                # mtx - the camera calibration's intrinsics
                good, prvecs, ptvecs = cv2.solvePnP(object_pts, imagePoints, mtx, None, flags=cv2.SOLVEPNP_ITERATIVE)
             #   imgpts, jac = cv2.projectPoints(opoints, prvecs, ptvecs, mtx, dcoeffs)

                # Draws the edges of the pose onto the image
                #print(str(prvecs) + '\n' + str(ptvecs) + '\n\n\n')

                print(f'pitch:{prvecs[0]}')
                print(f'yaw:{prvecs[1]}')
                print(f'roll:{prvecs[2]}') #

                print(f'left right:{ptvecs[0]-4}')
                print(f'up down:{ptvecs[1]}')
                print(f'distance:{ptvecs[2]}')
                print()

                table.putNumber("up_down", ptvecs[1])
                table.putNumber("left_right", ptvecs[0]-4)
                table.putNumber("distance", ptvecs[2])
                table.putNumber("roll", prvecs[2])
                table.putNumber("pitch", prvecs[0])
                table.putNumber("yaw", prvecs[1])

                #sys.stdout.write('\a')