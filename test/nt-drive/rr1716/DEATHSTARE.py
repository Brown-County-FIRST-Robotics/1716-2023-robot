#!/usr/bin/env python

import logging
from flask import Flask, render_template, Response, redirect, request
from threading import Thread
import rr1716
import cv2
import numpy as np

__COLOR_PICK_RANGE__ = 20

app = Flask(__name__)

temp_cameras = []

# This function gets called by the /video_feed route below
def gen_frames(camera):  # generate frame by frame from camera
    logging.debug("DEATHSTARE.gen_frames")
    # We want to loop this forever
    while True:
        camera.clear_rectangle()
        frame = camera.get_jpg_bytes()
        # Return the image to the browser
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')  # concat frame one by one and show result

@app.route('/camera_feed/<int:Number>')
def camera_feed(Number):
    logging.debug("DEATHSTARE.camera_feed")

    if len(app.Cameras) <= Number:
        the_camera = app.Cameras[-1]
    else:
        the_camera = app.Cameras[Number]

    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames(the_camera), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/return')
def returnToIndex():
    global app
    logging.debug("DEATHSTARE.return")
    for i in range(len(app.Cameras)):
        app.Cameras[i].clear_rectangle()
    """Video streaming home page."""
    return redirect('/')

@app.route('/')
def index():
    logging.debug("DEATHSTARE.index")
    """Video streaming home page."""
    return render_template('sidecam.html')

#Color picker
# This function gets called by the /video_feed route below
def gen_frames_picker(camera):  # generate frame by frame from camera
    logging.debug("DEATHSTARE.gen_frames_picker")
    
    w = camera.get_width()
    h = camera.get_height()
    x = int(w / 2)
    y = int(h / 2) 
    # We want to loop this forever
    while True: 
        camera.add_rectangle((x - __COLOR_PICK_RANGE__, y - __COLOR_PICK_RANGE__),
                         (x + __COLOR_PICK_RANGE__, y + __COLOR_PICK_RANGE__),
                         (0, 255, 0), 1)
        frame = camera.get_jpg_bytes()

        #print("dimensions", len(frame), len(frame[0]))

        # Return the image to the browser
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')  # concat frame one by one and show result

@app.route('/picker_image')
def picker_image():
    logging.debug("DEATHSTARE.picker_image")

    the_camera = app.Cameras[0]

    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames_picker(the_camera), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/picker')
def picker_page():
    logging.debug("DEATHSTARE.picker")
    return render_template('picker.html')

#Color picker
# This function gets called by the /video_feed route below
def gen_preview_picker(camera):  # generate frame by frame from camera
    logging.debug("DEATHSTARE.gen_frames_picker")

    col = []
    with open("picked_color") as file:
        for line in file:
            for x in line.split():
                col.append(int(x)) 

    gameobj = rr1716.Vision.GamePiece()
    lower = [col[0] * 0.5, col[1] * 0.5, col[2] * 0.1]
    upper = [col[0] * 1.4, col[1] * 1.4, col[2] * 4.0]

    for i in range(len(lower)):
        if lower[i] < 0:
            lower[i] = 0
        if lower[i] > 255:
            lower[i] = 255

        if upper[i] < 0:
            upper[i] = 0
        if upper[i] > 255:
            upper[i] = 255

    gameobj.setLowerColor(np.array(lower, dtype=np.uint8))
    gameobj.setUpperColor(np.array(upper, dtype=np.uint8))

    # We want to loop this forever
    while True:
        hsv = camera.get_hsv()
        frame = camera.get_frame()

        gameobj.findCone(frame)
        #print(gameobj.getX(), gameobj.getY())
        camera.add_rectangle([ int(gameobj.getX() - gameobj.getWidth() / 2), int(gameobj.getY() - gameobj.getHeight() / 2) ],
                             [ int(gameobj.getX() + gameobj.getWidth() / 2), int(gameobj.getY() + gameobj.getHeight() / 2) ],
                             [ 0, 255, 0 ], 2)
        #gameobj.drawBoundRect(frame, [0,255,0])
        data = camera.get_jpg_bytes() 
        
        """
        print(gameobj.getLowerColor(), gameobj.getUpperColor())

        hsv = camera.get_hsv()
        frame = camera.get_frame()
        imgmask = cv2.inRange(hsv, gameobj.getLowerColor(), gameobj.getUpperColor())
        res = cv2.bitwise_and(frame, frame, mask = imgmask)
        imgray = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)
        ret, thresh = cv2.threshold(imgray, 127, 255, 0)

        ret, jpeg = cv2.imencode('.jpg', imgmask)
        data = jpeg.tobytes()
        """

        # Return the image to the browser
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + data + b'\r\n')  # concat frame one by one and show result

@app.route('/preview')
def preview_page():
    logging.debug("DEATHSTARE.preview")
    return render_template('preview.html')

@app.route('/preview_image')
def preview_image():
    logging.debug("DEATHSTARE.preview_image")
    the_camera = app.Cameras[0]
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_preview_picker(the_camera), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route("/pick")
def pick():
    app.Cameras[0].get_frame()
    col = rr1716.Vision.averageColor(app.Cameras[0].get_hsv(), __COLOR_PICK_RANGE__)
    print("color: ", col) 
    #write to file
    file = open("picked_color", "w")
    file.write(str(int(col[0])) + " " + str(int(col[1])) + " " + str(int(col[2])) + " \n")
    file.close()
    return redirect("/preview")

def start(camera):
    logging.debug("DEATHSTARE.start")
    app.Cameras = camera
    thread = Thread(target=app.run, kwargs={'host':"0.0.0.0"})
    thread.start()

if __name__ == '__main__':
    pass
else:
    pass
