from flask import Flask, render_template, Response, redirect, request
import cv2
import threading
import numpy as np
import json, sys, subprocess, os, glob
import apriltagModule
import asyncio

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


def readCam(cam, frames, ind):
    rawCameraMatrix = np.array(calibrations[ind]['cameraMatrix'])
    distCoeffs = np.array(calibrations[ind]['cameraDistortion'])
    res = [int(cam.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cam.get(cv2.CAP_PROP_FRAME_HEIGHT))]
    cameraMatrix, roi = cv2.getOptimalNewCameraMatrix(rawCameraMatrix, distCoeffs, res, 1,
                                                      res)
    map1, map2 = cv2.initUndistortRectifyMap(rawCameraMatrix, distCoeffs, None, cameraMatrix,
                                             res, cv2.CV_16SC2)

    while True:
        ret, frame = cam.read()
        if not ret:
            break
        cv2.remap(frame, map1, map2, cv2.INTER_CUBIC)
        res = apriltagModule.getPosition(frame, np.array([[1.31927534e+03, 0.00000000e+0, 1.04468063e+03],
                                                          [0.00000000e+00, 1.32853937e+03, 5.66297124e+02],
                                                          [0.00000000e+00, 0.00000000e+00, 1.00000000e+00]]), None)
        if res is not None and len(res) > 0:
            print([i.yaw for i in res])
        resized = cv2.resize(frame,
                             (int(240 / cam.get(cv2.CAP_PROP_FRAME_HEIGHT) * cam.get(cv2.CAP_PROP_FRAME_WIDTH)), 240),
                             interpolation=cv2.INTER_LINEAR)
        frames[ind] = resized


def openCam(i):
    global cameras
    cam = cv2.VideoCapture()
    cam.open(cameraDev[i])
    assert tuple(calibrations[i]["calibrationResolution"]) == (
        cam.get(cv2.CAP_PROP_FRAME_WIDTH), cam.get(cv2.CAP_PROP_FRAME_HEIGHT))
    cam.set(cv2.CAP_PROP_FRAME_WIDTH, 20)
    cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 15)
    cameras.append(cam)


async def getCams():
    global cameras
    for i in range(len(cameraDev)):
        th = threading.Thread(target=openCam, args=(i,))
        th.start()
    while len(cameras) < len(cameraDev):
        await asyncio.sleep(0.05)  # print("Capturing all cameras...")
    print('cameras good')
    return cameras


@app.route('/')
def index():
    """Video streaming home page."""
    return redirect('/goto_allcam')


@app.route('/goto_allcam')
async def gotoAllCam():
    await getCams()
    return redirect('/allcam')


@app.route('/allcam')
def all():
    return render_template("allcam.html")


def showAllCams():
    global imageHoriz
    for i in range(len(cameraDev)):
        th = threading.Thread(target=readCam, args=(cameras[i], imageHoriz, i))
        th.start()

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
    cam = cv2.VideoCapture()
    cam.open(cameraDev[ind])
    cam.set(cv2.CAP_PROP_FRAME_WIDTH, 20)
    cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 15)
    cameras.append(cam)
    while True:
        ret, frame = cam.read()
        # This step encodes the data into a jpeg image 
        ret, buffer = cv2.imencode('.jpg', frame)

        # We have to return bytes to the user
        img = buffer.tobytes()

        # Return the image to the browser
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + img + b'\r\n')


if __name__ == '__main__':
    for i in range(len(cameraDev)):
        imageHoriz.append(np.zeros((240, 240, 3), dtype=np.uint8))
    app.run(threaded=True, host="0.0.0.0")
