#!/usr/bin/env python

import logging
import cv2
import time
import numpy as np

#Cameras = CameraTracker()

class Camera:
    def __init__(self, device, calibration, position):
        logging.debug("camera.init")
        self.frame = None
        self.hsv = None
        self.gray = None
        self.id = None
        self.device = device
        self.camera = cv2.VideoCapture(device)

        self.pos=position

        video_size = calibration["calibrationResolution"]
        self.camera.set(cv2.CAP_PROP_FRAME_WIDTH, video_size[0])
        self.camera.set(cv2.CAP_PROP_FRAME_HEIGHT, video_size[1])
        test_video_size = (self.camera.get(cv2.CAP_PROP_FRAME_WIDTH), self.camera.get(cv2.CAP_PROP_FRAME_HEIGHT))
        assert tuple(test_video_size) == tuple(video_size), 'camera resolution didnt set'

        raw_camera_matrix = np.array(calibration['cameraMatrix'])
        dist_coefficients = np.array(calibration['cameraDistortion'])
        processing_resolution = np.array(calibration['processingResolution'])

        self.camera_matrix, roi = cv2.getOptimalNewCameraMatrix(raw_camera_matrix, dist_coefficients, video_size, 0,
                                                           processing_resolution)
        self.map1, self.map2 = cv2.initUndistortRectifyMap(raw_camera_matrix, dist_coefficients, None, self.camera_matrix,
                                                 processing_resolution, cv2.CV_16SC2)

        self.camera.set(cv2.CAP_PROP_FPS, 10)
        self.camera.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))


    def update(self):
        logging.debug("Camera.update")
        success, frame = self.camera.read()
        frame = cv2.remap(frame, self.map1, self.map2, cv2.INTER_CUBIC)
        if success:
            self.frame = frame
            self.hsv = None
            self.gray = None
        else:
            logging.critical("Camera Read Failed %s" % self.device)

    def get_frame(self, flipped=False):
        if flipped:
            return cv2.flip(self.frame, 1)
        else:
            return self.frame

    def get_hsv(self):
        logging.debug("camera.get_hsv")
        if self.hsv is None:
            self.hsv = cv2.cvtColor(self.frame, cv2.COLOR_BGR2HSV)
        return self.hsv

    def get_gray(self):
        logging.debug("camera.get_gray")
        if self.gray is None:
            self.gray = cv2.cvtColor(self.frame, cv2.COLOR_BGR2GRAY)
        return self.gray

    def get_height(self):
        logging.debug("camera.get_height")
        return len(self.frame)

    def get_width(self):
        logging.debug("camera.get_width")
        return len(self.frame)

    def add_rectangle(self, start, end, color, thickness=2):
        logging.debug("camera.add_rectangle")
        self.frame = cv2.rectangle(self.frame, start, end, color, thickness)

    def get_jpg_bytes(self, flipped=False):
        logging.debug("camera.get_jpg_bytes")
        frame = self.get_frame(flipped)
        ret, buffer = cv2.imencode('.jpg', frame)
        jpg = buffer.tobytes()
        return jpg
'''
def list_ports():
    
    #Test the ports and returns a tuple with the available ports and the ones that are working.
    
    non_working_ports = []
    dev_port = 0
    working_ports = []
    available_ports = []
    while len(non_working_ports) < 6: # if there are more than 5 non working ports stop the testing. 
        camera = cv2.VideoCapture(dev_port)
        if not camera.isOpened():
            non_working_ports.append(dev_port)
            logging.debug("Port %s is not working." %dev_port)
        else:
            is_reading, img = camera.read()
            w = camera.get(3)
            h = camera.get(4)
            if is_reading:
                logging.debug("Port %s is working and reads images (%s x %s)" %(dev_port,h,w))
                working_ports.append(dev_port)
            else:
                logging.debug("Port %s for camera ( %s x %s) is present but does not reads." %(dev_port,h,w))
                available_ports.append(dev_port)
            camera.release()
        dev_port +=1
    return working_ports
'''

if __name__ == "__main__":
    # We're a module, never run anything here
    pass
else:
    pass
    # Run things on import here
    #cams = list_ports()

    
