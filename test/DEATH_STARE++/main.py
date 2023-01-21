"""
TODO:remove old apritag/color
TODO:fix camera bugs, add calibration files
TODO:move calibration files
"""

#flask is the web server so we can run this app in the browser
from flask import Flask, render_template, Response, redirect, request
import cv2
import threading
#import april
import numpy as np
import json, sys, subprocess, os, glob
import apriltagModule
import asyncio

if sys.argv[1]=='--config':
    print('making config file')
    config={}
    for i in range(4):
        config[f'camera_{i}']={}
        while True:
            camera_port=input(f'camera {i+1} port(probably an even number):')
            if not camera_port.isdigit():
                print('please input a number')
                continue
            if not os.path.exists(f'/dev/video{camera_port}'):
                print('camera not plugged in')
                continue
            break
        camera_id=subprocess.getoutput(f'udevadm info -q all -n /dev/video{camera_port} | grep -i -P "^e: id_model_id"')[15:]
        """
        How to get further camera information:
        run `udevadm info -q all -n /dev/video{cam port} | grep -i -P '$e: v4l/by-path/.*-index'`
        
        If it returns 0, the camera is good
        """

        assert len(camera_id)==4, 'invalid id. This is a weird camera problem. '
        config[f'camera_{i}']['uid']=camera_id
        while True:
            calibration_fname=input('camera calibration file(glob patterns allowed):')
            if len(glob.glob(calibration_fname))!=1:
                print(f'{calibration_fname} does not exit, or glob pattern is not specific enough. ')
                continue
            break
        with open(calibration_fname, 'r') as calibration_file:
            calibration = json.loads(calibration_file.read())
            '''
            add calibration code here
            '''
            config[f'camera_{i}']['calibration']=calibration
        while True:
            pos=json.loads(input(f'Position of camera {i+1}(x,y,z,yaw,pitch,roll):'))
            if len(pos)!=6:
                print('Length=6')
                continue
            break
        config[f'camera_{i}']['pos']=pos
    config_fname=input('Name of config file:')
    assert input(f'This will create a file called "configs/{config_fname}.json", are you sure(y/n):')=='y', 'Operation canceled'
    with open(f'configs/{config_fname}.json','w') as config_file:
        config_file.write(json.dumps(config))
    print('Config file made')
    sys.exit()

if len(sys.argv)!=3:
    print('Run "python3 main.py {config file name} {server IP address}"')
    print()
    print('If you want to make a config file, run "python3 main.py --config"')
    sys.exit()



app = Flask(__name__)
app.config["SECRET_KEY"] = "1716robotics"

cameraDev = [ "/dev/video5", "/dev/video0"]#, "/dev/video0", "/dev/video0", "/dev/video0" ]
#camera.open(cameraDev[0])


cameras = []
imageHoriz = []

#camera dimensions
#camWidth = camera.get(cv2.CAP_PROP_FRAME_WIDTH)
#camHeight = camera.get(cv2.CAP_PROP_FRAME_HEIGHT)

currentCam = 0

displayApril = False 
displayColor = False

cols = [ [ 0, 0, 0 ], [ 0, 0, 0 ], [ 255, 255, 255 ]]
#currentFrame = camera.read()

def readCam(cam, frames, ind):
    while True:
        ret, frame = cam.read()
        if not ret:
            break
        resized = cv2.resize(frame, 
                    (int(240 / cam.get(cv2.CAP_PROP_FRAME_HEIGHT) * cam.get(cv2.CAP_PROP_FRAME_WIDTH)), 240),
                    interpolation=cv2.INTER_LINEAR)
        frames[ind] = resized

def openCam(i):
    global cameras
    cam = cv2.VideoCapture() 
    cam.open(cameraDev[i]) 
    cam.set(cv2.CAP_PROP_FRAME_WIDTH, 20)
    cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 15)
    cameras.append(cam)


async def getCams():
    global cameras
    for i in range(len(cameraDev)):
        print('hi')
        th = threading.Thread(target=openCam, args=(i,))
        th.start()
    while len(cameras) < len(cameraDev):
        await asyncio.sleep(0.05)#print("Capturing all cameras...")

    print('cameras good')
    return cameras

"""
def getSpecificCams(indices):
    global imageHoriz
    cams = []
    for i in range(len(indices)):
        th = threading.Thread(target=openCam, args=(indices[i], cams))
        th.start()
    while len(cams) < len(indices):
        print("Capturing cameras...")
    for i in range(len(cams)):
        th = threading.Thread(target=readCam, args=(cams[i], imageHoriz, i))
        th.start()
    return cams
"""

# This function gets called by the /video_feed route below
def gen_frames():  # generate frame by frame from camera
    global currentFrame

    # We want to loop this forever
    while True:
        # Capture frame-by-frame
        success, frame = camera.read()  # read the camera frame

        #display april tags
        if displayApril:
            april.displayApril(frame, camera)
        if displayColor:
            color.findColor(frame, camera, np.array(cols[1], dtype=np.uint8),
                                           np.array(cols[2], dtype=np.uint8))

        
        #draw crosshairs
        cv2.rectangle(frame, 
                      (int(camWidth / 2 - 2), int(camHeight / 2 - 16)),
                      (int(camWidth / 2 + 2), int(camHeight / 2 + 16)),
                      [255, 0, 0],2)
        cv2.rectangle(frame, 
                      (int(camWidth / 2 - 16), int(camHeight / 2 - 2)),
                      (int(camWidth / 2 + 16), int(camHeight / 2 + 2)),
                      [255, 0, 0],2)

        # If something goes wrong with the camera, exit the function
        if not success:
            break
        
        currentFrame = frame

        # This step encodes the data into a jpeg image
        ret, buffer = cv2.imencode('.jpg', frame)

        # We have to return bytes to the user
        frame = buffer.tobytes() 

        # Return the image to the browser
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')  # concat frame one by one and show result


@app.route('/video_feed')
def video_feed():
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
def index():
    """Video streaming home page."""
    return render_template('index.html')

#switch between cameras
@app.route('/next')
def next():
    global currentCam
    global camera
    global camWidth
    global camHeight

    currentCam += 1
    currentCam %= len(cameraDev) 
    camera.release()
    camera = cv2.VideoCapture()
    camera.open(cameraDev[currentCam])
    camera.set(cv2.CAP_PROP_FRAME_WIDTH, 20)
    camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 15)

    print(currentCam) 
    
    camWidth = camera.get(cv2.CAP_PROP_FRAME_WIDTH)
    camHeight = camera.get(cv2.CAP_PROP_FRAME_HEIGHT)

    return redirect('/')

@app.route('/toggle_april_tag')
def toggle_april():
    global displayApril
    displayApril = not displayApril
    return redirect('/')

@app.route('/toggle_color_detection')
def toggle_color():
    global displayColor
    displayColor = not displayColor
    return redirect('/')

@app.route('/prev')
def prev():
    global currentCam
    global camera
    global camWidth
    global camHeight    

    currentCam -= 1
    if currentCam < 0:
        currentCam = len(cameraDev) - 1 
    print(currentCam)
    camera.release()
    camera = cv2.VideoCapture()
    camera.open(cameraDev[currentCam])
    camera.set(cv2.CAP_PROP_FRAME_WIDTH, 20)
    camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 15) 

    camWidth = camera.get(cv2.CAP_PROP_FRAME_WIDTH)
    camHeight = camera.get(cv2.CAP_PROP_FRAME_HEIGHT)

    return redirect('/')


@app.route('/goto_allcam')
async def gotoAllCam():
    await getCams()
    return redirect('/allcam')

@app.route('/allcam')
def all():
    return render_template("allcam.html")

def showAllCams():
    global imageHoriz
    print('hiiii',len(cameras))
    for i in range(len(cameraDev)):
        th = threading.Thread(target=readCam, args=(cameras[i], imageHoriz, i))
        th.start()

    # We want to loop this forever
    while True:

        allImages = cv2.hconcat(imageHoriz)

        # This step encodes the data into a jpeg image 
        ret, buffer = cv2.imencode('.jpg', allImages)

        # We have to return bytes to the user
        allImages = buffer.tobytes() 

        # Return the image to the browser
        yield (b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n\r\n' + allImages + b'\r\n')  # concat frame one by one and show result

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

#side camera view


#@app.route("/side1")
#def side1():
#    return Response(readImg(0), mimetype='multipart/x-mixed-replace; boundary=frame')
#@app.route("/side2")
#def side2():
#    return Response(readImg(1), mimetype='multipart/x-mixed-replace; boundary=frame')
#@app.route("/side3")
#def side3():
#    return Response(readImg(2), mimetype='multipart/x-mixed-replace; boundary=frame')
#@app.route("/side4")
#def side4():
#    return Response(readImg(3), mimetype='multipart/x-mixed-replace; boundary=frame')


if __name__ == '__main__':
    for i in range(len(cameraDev)):
        imageHoriz.append(np.zeros((240, 240, 3), dtype=np.uint8))
    app.run(threaded=True, host="0.0.0.0")
