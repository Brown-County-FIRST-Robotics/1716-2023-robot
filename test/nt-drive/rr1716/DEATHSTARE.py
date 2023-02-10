#!/usr/bin/env python

import logging
from flask import Flask, render_template, Response, redirect, request
from threading import Thread
import rr1716
import cv2

__COLOR_PICK_RANGE__ = 20

app = Flask(__name__)

temp_cameras = []

# This function gets called by the /video_feed route below
def gen_frames(camera):  # generate frame by frame from camera
    logging.debug("DEATHSTARE.gen_frames")
    # We want to loop this forever
    while True:

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

@app.route('/')
def index():
    logging.debug("DEATHSTARE.index")
    """Video streaming home page."""
    return render_template('sidecam.html')

@app.route('/picker')
def picker_page():
    return render_template('picker.html')

@app.route("/pick")
def pick():
    col = rr1716.Vision.averageColor(cv2.cvtColor(app.Cameras[0].frame, cv2.COLOR_BGR2HSV),
                                                  __COLOR_PICK_RANGE__)
    print("color: ", col)
    #write to file
    file = open("picked_color", "w")
    file.write(str(int(col[0])) + " " + str(int(col[1])) + " " + str(int(col[2])) + " \n")
    file.close()
    return redirect("/")

def start(camera):
    logging.debug("DEATHSTARE.start")
    app.Cameras = camera
    thread = Thread(target=app.run, kwargs={'host':"0.0.0.0"})
    thread.start()

if __name__ == '__main__':
    pass
else:
    pass
