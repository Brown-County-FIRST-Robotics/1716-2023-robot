from flask import Flask, render_template, Response, redirect, request
import cv2
import threading
import numpy as np
import json, sys, subprocess, os, glob
import apriltagModule
import asyncio
from networktables import NetworkTables



global config

if sys.argv[1] == '--config':
    print('making config file')
    config = []
    for i in range(int(input('Number of cameras:'))):
        config.append({})
        while True:
            camera_port = input(f'camera {i + 1} port(probably an even number):')
            if not camera_port.isdigit():
                print('please input a number')
                continue
            if not os.path.exists(f'/dev/video{camera_port}'):
                print('camera not plugged in')
                continue
            break
        camera_id = subprocess.getoutput(
            f'udevadm info -q all -n /dev/video{camera_port} | grep -i -P "^e: id_model_id"')[15:]
        """
        How to get further camera information:
        run `udevadm info -q all -n /dev/video{cam port} | grep -i -P '$s: v4l/by-path/.*-index'`
        
        If it returns 0, the camera is good
        """

        assert len(camera_id) == 4, 'invalid id. This is a weird camera problem. '
        config[-1]['uid'] = camera_id
        while True:
            calibration_fname = input('camera calibration file(glob patterns allowed):')
            if len(glob.glob(calibration_fname)) != 1:
                print(f'{calibration_fname} does not exit, or glob pattern is not specific enough. ')
                continue
            break
        with open(glob.glob(calibration_fname)[0], 'r') as calibration_file:
            calibration = json.loads(calibration_file.read())
            config[-1]['calibration'] = calibration
        while True:
            pos = input(f'Position of camera {i + 1}(x,y,z,yaw,pitch,roll):')
            if pos == '':
                pos = '[0,0,0,0,0,0]'
            pos = json.loads(pos)
            if len(pos) != 6:
                print('Length=6')
                continue
            break
        config[-1]['pos'] = pos
    config_fname = input('Name of config file:')
    assert input(
        f'This will create a file called "configs/{config_fname}.json", are you sure(y/n):') == 'y', 'Operation canceled'
    with open(f'configs/{config_fname}.json', 'w') as config_file:
        config_file.write(json.dumps(config))
    print('Config file made')
    sys.exit()

if len(sys.argv) != 3:
    print('Run "python3 main.py {config file name} {server IP address}"')
    print()
    print('If you want to make a config file, run "python3 main.py --config"')
    sys.exit()

NetworkTables.initialize(server=sys.argv[-1])

april_table = NetworkTables.getTable("apriltag")


app = Flask(__name__)
app.config["SECRET_KEY"] = "1716robotics"

with open(sys.argv[1], 'r') as config_file:
    config = json.loads(config_file.read())

cameraDev = []
calibrations = []

for i in config:
    for camera in glob.glob('/dev/video*'):
        if subprocess.getoutput(f"udevadm info -q all -n {camera} | grep -i -P '^s: v4l/by-path/.*-index'")[-1] == '0':
            if subprocess.getoutput(f'udevadm info -q all -n {camera} | grep -i -P "^e: id_model_id"')[15:] == i['uid']:
                cameraDev.append(camera)
                calibrations.append(i['calibration'])
                break

cameras = []
imageHoriz = []


def handleApriltags(image, cameraMatrix):
    res = apriltagModule.getPosition(image, cameraMatrix, None)

    if res is not None and len(res) > 0:
        for i in res:
            april_table.putNumber("up_down", i.up_down)
            april_table.putNumber("left_right", i.left_right)
            april_table.putNumber("distance", i.distance)
            april_table.putNumber("yaw", i.yaw)




def handleCam(ind):
    global cameras
    global imageHoriz


    cam = cv2.VideoCapture()
    cam.open(cameraDev[ind])
    # it is possible to scale rawCameraMatrix by the calibrationResolution and captureResolution, see https://stackoverflow.com/questions/44888119/c-opencv-calibration-of-the-camera-with-different-resolution
    # instead here we just make sure they are the same
    video_size = calibrations[ind]["calibrationResolution"]
    cam.set(cv2.CAP_PROP_FRAME_WIDTH, video_size[0])
    cam.set(cv2.CAP_PROP_FRAME_HEIGHT, video_size[1])
    test_video_size = (cam.get(cv2.CAP_PROP_FRAME_WIDTH), cam.get(cv2.CAP_PROP_FRAME_HEIGHT))
    assert tuple(test_video_size) == tuple(video_size), 'camera resolution didnt set'

    rawCameraMatrix = np.array(calibrations[ind]['cameraMatrix'])
    distCoeffs = np.array(calibrations[ind]['cameraDistortion'])
    processingResolution = np.array(calibrations[ind]['processingResolution'])

    cameraMatrix, roi = cv2.getOptimalNewCameraMatrix(rawCameraMatrix, distCoeffs, video_size, 0, processingResolution)
    map1, map2 = cv2.initUndistortRectifyMap(rawCameraMatrix, distCoeffs, None, cameraMatrix, processingResolution, cv2.CV_16SC2)

    cameras[ind] = cam

    while True:
        ret, frame = cam.read()
        if not ret:
            break

        #undistort
        frame = cv2.remap(frame, map1, map2, cv2.INTER_CUBIC)

        #april tags
        handleApriltags(frame, cameraMatrix)

        #for sending to the web user
        resized = cv2.resize(frame,
                             (int(240 / cam.get(cv2.CAP_PROP_FRAME_HEIGHT) * cam.get(cv2.CAP_PROP_FRAME_WIDTH)), 240),
                             interpolation=cv2.INTER_LINEAR)
        imageHoriz[ind] = resized






@app.route('/')
def index():
    """Video streaming home page."""
    return redirect('/goto_allcam')


@app.route('/goto_allcam')
async def gotoAllCam():
    return redirect('/allcam')


@app.route('/allcam')
def all():
    return render_template("allcam.html")


def showAllCams():
    global imageHoriz

    # We want to loop this forever
    while True:
        allImages = cv2.hconcat(imageHoriz)
        if not imageHoriz:
            continue
        # This step encodes the data into a jpeg image 
        ret, buffer = cv2.imencode('.webp', allImages)

        # We have to return bytes to the user
        allImages = buffer.tobytes()

        # Return the image to the browser
        yield (b'--frame\r\n'
               b'Content-Type: image/webp\r\n\r\n' + allImages + b'\r\n')  # concat frame one by one and show result


@app.route('/allCamsImage')
def allCamsImage():
    return Response(showAllCams(), mimetype='multipart/x-mixed-replace; boundary=frame')


def readImg(ind):
    while True:
        # This step encodes the data into a webp image
        ret, buffer = cv2.imencode('.webp', imageHoriz[ind])

        # Return the image to the browser
        yield (b'--frame\r\n'
               b'Content-Type: image/webp\r\n\r\n' + buffer.tobytes() + b'\r\n')


@app.route("/camera/<num>")
def camera(num):
    return Response(readImg(int(num)), mimetype='multipart/x-mixed-replace; boundary=frame')


def main():
    for i in range(len(cameraDev)):
        imageHoriz.append(np.zeros((240, 240, 3), dtype=np.uint8))

    global cameras
    cameras = [None] * len(cameraDev)
    for i in range(len(cameraDev)):
        th = threading.Thread(target=handleCam, args=(i,))
        th.start()

    app.run(threaded=True, host="0.0.0.0")


if __name__ == '__main__':
    main()
