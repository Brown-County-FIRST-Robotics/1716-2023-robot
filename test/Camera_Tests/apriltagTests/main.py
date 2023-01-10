# (x,y,z, roll, pitch, yaw)
'''
------ <default position
y/roll
|
|
. ____x/pitch
z/yaw
'''
import time

import apriltag
import cv2
import numpy as np
import sys, os
import random
import json

# I am getting the tag sizes from a json file for all apriltags

# tag_sizes, tvecs, rvecs = get_extrinsics(json_file)

cam = cv2.VideoCapture()
cam.open('/dev/video3')

tag_size = 8/2.54

video_size = (cam.get(cv2.CAP_PROP_FRAME_WIDTH), cam.get(cv2.CAP_PROP_FRAME_HEIGHT))

print(video_size)

#focal_length = video_size[1]
#camera_center = (video_size[1] / 2, video_size[0] / 2)

f=open('camera_calibrations/ms.json','r')
mtx,dist=json.loads(f.read())
print(mtx)
mtx=np.array(mtx)
dist=np.array(dist)

def convert(pt):
    return (int(pt[0]), int(pt[1]))


apriltags_found=0
data=[]
while apriltags_found<500:
    img = cam.read()[1]

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
                good, prvecs, ptvecs = cv2.solvePnP(object_pts, imagePoints, mtx, dist, flags=cv2.SOLVEPNP_ITERATIVE)
             #   imgpts, jac = cv2.projectPoints(opoints, prvecs, ptvecs, mtx, dcoeffs)

                # Draws the edges of the pose onto the image
                #print(str(prvecs) + '\n' + str(ptvecs) + '\n\n\n')
                apriltags_found+=1
                os.system('clear')
                print(f'pitch:{prvecs[0]}')
                print(f'yaw:{prvecs[1]}')
                print(f'roll:{prvecs[2]}') #

                print(f'left right:{-ptvecs[0]-ptvecs[2]/4}')
                print(f'up down:{(ptvecs[1]+ptvecs[2]/16)*2}')
                print(f'distance:{ptvecs[2]}')
                print()
                #sys.stdout.write("\a")
                data.append([prvecs[0],
                             prvecs[1],
                             prvecs[2],
                             -ptvecs[0]-ptvecs[2]/4,
                             (ptvecs[1]+ptvecs[2]/16)*2,
                             ptvecs[2]])
    #time.sleep(0.1)
pitch=[float(i[0]) for i in data]
yaw=[float(i[1]) for i in data]
roll=[float(i[2]) for i in data]
left_right=[float(i[3]) for i in data]
up_down=[float(i[4]) for i in data]
distance=[float(i[5]) for i in data]

f=open('data.json','r')
data=json.loads(f.read())
f.close()
data.append([
    [np.average(pitch),np.average(yaw),np.average(left_right),np.average(up_down),np.average(distance)],
    [np.std(pitch),np.std(yaw),np.std(left_right),np.std(up_down),np.std(distance)]
])
f=open('data.json','w')
f.write(json.dumps(data))
f.close()
