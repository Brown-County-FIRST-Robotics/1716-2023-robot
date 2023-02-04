#!/usr/bin/env python

import logging
from flask import Flask, render_template, Response, redirect, request
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
    return render_template('index.html')

@app.route('/all')
def all():
    logging.debug("DEATHSTARE.all")

    number = request.args.get('num')
    camera = request.args.get('cam')
    if number == None:
        number = 0
    else:
        number = int(number)
    if camera is not None:
        app.Cameras[int(camera)].id = number

    if number == 4:
        return redirect("/")
    return render_template('all.html', len=5, num=number+1)

def start(camera):
    logging.debug("DEATHSTARE.start")
    app.Cameras = camera
    thread = Thread(target=app.run)
    thread.start()

if __name__ == '__main__':
    pass
else:
    pass
