#flask is the web server so we can run this app in the browser 
from flask import Flask, render_template, Response, redirect
import cv2
import threading

app = Flask(__name__)

CAM_COUNT = 2
MAX_PORT_SCAN = 1000
cameraIds = [ 0 ]
camIndex = 0
camera = cv2.VideoCapture(0)

#camera dimensions
camWidth = camera.get(cv2.CAP_PROP_FRAME_WIDTH)
camHeight = camera.get(cv2.CAP_PROP_FRAME_HEIGHT)

currentCam = 0

def getCamIds():
    #Attempt to scan all dev ports
    camsFound = 0
    for i in range(MAX_PORT_SCAN):
        tempCam = cv2.VideoCapture(i)
        #Found camera 
        if tempCam.isOpened():
            cameraIds.append(i)
            camsFound += 1

        if camsFound >= CAM_COUNT:
            break;


# This function gets called by the /video_feed route below
def gen_frames():  # generate frame by frame from camera
    # We want to loop this forever
    while True:
        # Capture frame-by-frame
        success, frame = camera.read()  # read the camera frame

        #draw crosshairs
        cv2.rectangle(frame, 
                      (int(camWidth / 2 - 2), int(camHeight / 2 - 16)),
                      (int(camWidth / 2 + 2), int(camHeight / 2 + 16)),
                      [255,0,0],2)
        cv2.rectangle(frame, 
                      (int(camWidth / 2 - 16), int(camHeight / 2 - 2)),
                      (int(camWidth / 2 + 16), int(camHeight / 2 + 2)),
                      [255,0,0],2)

        # If something goes wrong with the camera, exit the function
        if not success:
            break
        
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
    currentCam %= CAM_COUNT
    camera.release();
    camera = cv2.VideoCapture(cameraIds[currentCam]);
    print(currentCam) 
    
    camWidth = camera.get(cv2.CAP_PROP_FRAME_WIDTH)
    camHeight = camera.get(cv2.CAP_PROP_FRAME_HEIGHT)

    return redirect('/')

@app.route('/toggle_april_tag')
def toggle_april():
    return redirect('/');

@app.route('/toggle_color_detection')
def toggle_color():
    return redirect('/');

@app.route('/prev')
def prev():
    global currentCam
    global camera
    global camWidth
    global camHeight    

    currentCam -= 1
    if currentCam < 0:
        currentCam = CAM_COUNT - 1 
    print(currentCam)
    camera.release();
    camera = cv2.VideoCapture(cameraIds[currentCam]); 
    
    camWidth = camera.get(cv2.CAP_PROP_FRAME_WIDTH)
    camHeight = camera.get(cv2.CAP_PROP_FRAME_HEIGHT)

    return redirect('/')

if __name__ == '__main__':
    getCamIds()
    print(cameraIds)
    app.run(threaded=True)
