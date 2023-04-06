#!/usr/bin/env python

import logging
from flask import Flask, render_template, Response, redirect, request, jsonify, Blueprint, current_app
from threading import Thread
from rr1716 import AprilTags
import rr1716
import dataclasses

state_vis_bp = Blueprint('state_vis', __name__)

@state_vis_bp.route('/state')
def state():
    logging.debug("DEATHSTARE.state")
    return render_template('state.html')

@state_vis_bp.route('/stateest')
def stateest():
    logging.debug("DEATHSTARE.stateest")
    return jsonify(dataclasses.asdict(current_app.filter.current))

@state_vis_bp.route("/apriltags")
def get_apriltags():
    res=None
    for cam in current_app.Cameras:
        res=AprilTags.getPosition(cam.get_gray(), cam.camera_matrix, None)
        if res is not None and res!=[]:
            break
    if res:
        x,y,r=res[0].calcFieldPos()
        return Response(f'<h3>Field position: x:{x}, y:{y}, r:{r}</h3>', mimetype='text')
    return Response('<h3>No apriltags Found</h3>', mimetype='text')


@state_vis_bp.route("/apriltags2")
def get_apriltags2():
    res=[]
    for cam in current_app.Cameras:
        dets=AprilTags.getPosition(cam.get_gray(), cam.camera_matrix, None)
        for det in dets:
          print(det)
          x,y,r=det.calcFieldPos()
          out = (det.tagID, det.distance, det.left_right, det.yaw, x, y, r, det.RMSError)
          res.append(out)
    return jsonify(res)


if __name__ == '__main__':
    pass
else:
    pass
