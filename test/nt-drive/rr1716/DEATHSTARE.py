#!/usr/bin/env python

import logging
from flask import Flask, render_template, Response, redirect
from threading import Thread

app = Flask(__name__)

# This function gets called by the /video_feed route below
def gen_frames(camera):  # generate frame by frame from camera
    logging.debug("DEATHSTARE.gen_frames")
    # We want to loop this forever
    while True:

        frame = camera.get_jpg_bytes()
        # Return the image to the browser
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')  # concat frame one by one and show result


@app.route('/video_feed')
def video_feed():
    logging.debug("DEATHSTARE.video_feed")
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames(app.Cameras[0]), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/camera_feed0')
def camera_feed0():
    logging.debug("DEATHSTARE.camera_feed0")
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames(app.Cameras[0]), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/camera_feed1')
def camera_feed1():
    logging.debug("DEATHSTARE.camera_feed1")
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames(app.Cameras[1]), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/camera_feed2')
def camera_feed2():
    logging.debug("DEATHSTARE.camera_feed2")
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames(app.Cameras[2]), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/camera_feed3')
def camera_feed3():
    logging.debug("DEATHSTARE.camera_feed3")
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames(app.Cameras[3]), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/')
def index():
    logging.debug("DEATHSTARE.index")
    """Video streaming home page."""
    return render_template('index.html')

def start(camera):
    logging.debug("DEATHSTARE.start")
    app.open_cv = camera
    thread = Thread(target=app.run)
    thread.start()

if __name__ == '__main__':
    pass
else:
    pass