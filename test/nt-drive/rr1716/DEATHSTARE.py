#!/usr/bin/env python

import logging
from flask import Flask, render_template, Response, redirect, request, jsonify
from threading import Thread
from rr1716 import AprilTags
import cv2
import numpy as np
import rr1716
import dataclasses

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

@app.route('/state')
def state():
    logging.debug("DEATHSTARE.state")
    return render_template('state.html')

@app.route('/stateest')
def stateest():
    logging.debug("DEATHSTARE.stateest")
    return jsonify(dataclasses.asdict(app.filter.current))

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
@app.route('/preview')
def preview_page():
    logging.debug("DEATHSTARE.preview")
    return render_template('preview.html')

@app.route('/preview_image')
def preview_image():
    logging.debug("DEATHSTARE.preview_image")
    the_camera = app.Cameras[0]
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(rr1716.Vision.gen_preview_picker(the_camera), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route("/pick")
def pick():
    app.Cameras[0].get_frame()
    col = rr1716.Vision.averageColor(app.Cameras[0].get_color(), __COLOR_PICK_RANGE__)
    print("color: ", col) 
    #write to file
    file = open("picked_color", "w")
    file.write(str(int(col[0])) + " " + str(int(col[1])) + " " + str(int(col[2])) + " \n")
    file.close()
    return redirect("/preview")

@app.route("/pick_cone")
def pick_cone():
    app.Cameras[0].get_frame()
    col = rr1716.Vision.averageColor(app.Cameras[0].get_frame(), __COLOR_PICK_RANGE__)
    logging.info("cone color: " + str(col)) 
    #write to file
    file = open("cone_picked_color", "w")
    file.write(str(int(col[0])) + " " + str(int(col[1])) + " " + str(int(col[2])) + " \n")
    file.close()
    return redirect("/preview")

@app.route("/pick_cube")
def pick_cube():
    app.Cameras[0].get_frame()
    col = rr1716.Vision.averageColor(app.Cameras[0].get_frame(), __COLOR_PICK_RANGE__)
    logging.info("cube color: " + str(col)) 
    #write to file
    file = open("cube_picked_color", "w")
    file.write(str(int(col[0])) + " " + str(int(col[1])) + " " + str(int(col[2])) + " \n")
    file.close()
    return redirect("/preview")

@app.route("/apriltags")
def get_apriltags():
    res=None
    for cam in app.Cameras:
        res=AprilTags.getPosition(cam.get_gray(), cam.camera_matrix, None)
        if res is not None and res!=[]:
            break
    if res:
        x,y,r=res[0].calcFieldPos()
        return Response(f'<h3>Field position: x:{x}, y:{y}, r:{r}</h3>', mimetype='text')
    return Response('<h3>No apriltags Found</h3>', mimetype='text')

@app.route("/apriltags2")
def get_apriltags2():
    res=[]
    for cam in app.Cameras:
        dets=AprilTags.getPosition(cam.get_gray(), cam.camera_matrix, None)
        for det in dets:
          print(det)
          x,y,r=det.calcFieldPos()
          out = (det.tagID, det.distance, det.left_right, det.yaw, x, y, r, det.RMSError)
          res.append(out)
    return jsonify(res)


def start(camera, filter):
    logging.debug("DEATHSTARE.start")
    app.Cameras = camera
    thread = Thread(target=app.run, kwargs={'host':"0.0.0.0"})
    thread.start()
    app.filter=filter

if __name__ == '__main__':
    pass
else:
    pass
