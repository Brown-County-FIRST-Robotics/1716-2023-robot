#mostly coppied from https://github.com/kyle-bersani/opencv-examples/blob/master/CalibrationByCharucoBoard/CalibrateCamera.py
#with a little from https://mecaruco2.readthedocs.io/en/latest/notebooks_rst/Aruco/sandbox/ludovic/aruco_calibration_rotation.html



# System information:
# - Linux Mint 18.1 Cinnamon 64-bit
# - Python 2.7 with OpenCV 3.2.0

import numpy
import cv2
from cv2 import aruco
import json
import glob


# ChAruco board variables
CHARUCOBOARD_ROWCOUNT = 7
CHARUCOBOARD_COLCOUNT = 5 
ARUCO_DICT = aruco.Dictionary_get(aruco.DICT_5X5_1000)

# Create constants to be passed into OpenCV and Aruco methods
CHARUCO_BOARD = aruco.CharucoBoard_create(
        squaresX=CHARUCOBOARD_COLCOUNT,
        squaresY=CHARUCOBOARD_ROWCOUNT,
        squareLength=0.04,
        markerLength=0.02,
        dictionary=ARUCO_DICT)


import sys
if 'generate' in sys.argv[1:]:
    imboard=CHARUCO_BOARD.draw((2000, 2000))
    cv2.imwrite("chessboard.tiff", imboard)
    print('chessboard.tiff written')
    sys.exit()


# Create the arrays and variables we'll use to store info like corners and IDs from images processed
corners_all = [] # Corners discovered in all images processed
ids_all = [] # Aruco ids corresponding to corners discovered
image_size = None # Determined at runtime


camnum = int(sys.argv[1].strip())
if str(camnum) == sys.argv[1].strip():
    pass
cam = cv2.VideoCapture(camnum)
cam.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)
video_size = (cam.get(cv2.CAP_PROP_FRAME_WIDTH), cam.get(cv2.CAP_PROP_FRAME_HEIGHT))
as_int = tuple(int(x) for x in video_size)
assert as_int == video_size
video_size = as_int
print('size', video_size)

cv2.namedWindow("test")

img_counter = 0
corners_all = []
ids_all = []

while True:
    ret, img = cam.read()
    if not ret:
        print("failed to grab img")
        break

    dispimg = img.copy()
    proportion = max(dispimg.shape) / 1000.0
    dispimg = cv2.resize(dispimg, (int(dispimg.shape[1] / proportion), int(dispimg.shape[0] / proportion)))
    cv2.putText(dispimg, 'press space to capture', (50,200), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,0,0), 2, cv2.LINE_AA)

    cv2.imshow("test", dispimg)

    k = cv2.waitKey(1)
    if k%256 == 27:
        # ESC pressed
        print("Escape hit, breaking loop...")
        break


    print('captured frame')
    # Grayscale the image
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    image_size = gray.shape[::-1]

    # Find aruco markers in the query image
    corners, ids, _ = aruco.detectMarkers(image=gray, dictionary=ARUCO_DICT)

    # Outline the aruco markers found in our query image
    img = aruco.drawDetectedMarkers(image=img, corners=corners)

    # Get charuco corners and ids from detected aruco markers
    try:
        response, charuco_corners, charuco_ids = aruco.interpolateCornersCharuco(
            markerCorners=corners,
            markerIds=ids,
            image=gray,
            board=CHARUCO_BOARD)

        img = aruco.drawDetectedCornersCharuco(
            image=img,
            charucoCorners=charuco_corners,
            charucoIds=charuco_ids)

    except:
        response = 0
        # Draw the Charuco board we've detected to show our calibrator the board was properly detected

    proportion = max(img.shape) / 1000.0
    img = cv2.resize(img, (int(img.shape[1] / proportion), int(img.shape[0] / proportion)))

    if response > 20:
        #Add these corners and ids to our calibration arrays
        corners_all.append(charuco_corners)
        ids_all.append(charuco_ids)
        pass
    else:
        cv2.putText(img, 'Charuco not found', (50,50), cv2.FONT_HERSHEY_SIMPLEX, 1, (20,20,255), 2, cv2.LINE_AA)

    cv2.putText(img, 'press space to continue', (50,200), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,0,0), 2, cv2.LINE_AA)

    cv2.imshow("test", img)
    k = cv2.waitKey(0)
    if k % 256 == 27:
        # ESC pressed
        print("Escape hit, breaking loop...")
        break


    img_counter += 1

cam.release()

cv2.destroyAllWindows()

# Make sure at least one image was found
if img_counter < 5:
    # Calibration failed because there were not enough images, warn the user
    print("Calibration was unsuccessful. Need at least 5 images.")
    # Exit for failure
    exit()


# Now that we've seen all of our images, perform the camera calibration
# based on the set of points we've discovered
calibration, cameraMatrix, distCoeffs, rvecs, tvecs = aruco.calibrateCameraCharuco(
        charucoCorners=corners_all,
        charucoIds=ids_all,
        board=CHARUCO_BOARD,
        imageSize=image_size,
        cameraMatrix=None,
        distCoeffs=None)


# Print matrix and distortion coefficient to the console
out = {}
out["calibrationResolution"] = image_size
out["cameraMatrix"] = cameraMatrix
out["cameraDistortion"] = distCoeffs
print(out)
