import glob
import cv2, json
import numpy as np
import apriltagModule
with open(glob.glob('ap*/cam*/ms.json')[0], 'r') as f:
    mtx, dist = json.loads(f.read())
    mtx = np.array(mtx)
    dist = np.array(dist)


cam = cv2.VideoCapture()
cam.open('/dev/video0')
print(apriltagModule.getPosition(cam.read()[1], mtx, dist))
