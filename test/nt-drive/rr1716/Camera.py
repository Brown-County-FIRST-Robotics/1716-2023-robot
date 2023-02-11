#!/usr/bin/env python

import logging
import cv2
import time
import numpy as np

class Camera:
    def __init__(self, device, calibration, position):
        logging.debug("camera.init")
        self.frame = None
        self.hsv = None
        self.gray = None
        self.id = None
        self.frame_count = 0
        self.last_frame_count = 0
        self.device = device
        self.camera = cv2.VideoCapture(device)
        self.rectangles = None

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
            self.frame_count = self.frame_count + 1
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
        return len(self.frame[0])

    def add_rectangle(self, start, end, color, thickness=2):
        logging.debug("camera.add_rectangle")
        self.rectangles = [ start, end, thickness ]
        #self.frame = cv2.rectangle(self.frame, start, end, color, thickness)

    def clear_rectangle(self):
        logging.debug("camera.clear_rectangle")
        self.rectangles = None

    def get_jpg_bytes(self, flipped=False):
        # Let's block on this call if we alredy returned this frame
        while self.frame_count <= self.last_frame_count:
            time.sleep(0.01)
        self.last_frame_count = self.frame_count
        logging.debug("camera.get_jpg_bytes")
        frame = self.get_frame(flipped)
        
        if self.rectangles:
            cv2.rectangle(self.frame, self.rectangles[0], self.rectangles[1], (0, 255, 0), self.rectangles[2])

        ret, buffer = cv2.imencode('.jpg', frame)
        jpg = buffer.tobytes()
        return jpg

if __name__ == "__main__":
    # We're a module, never run anything here
    pass
else:
    # Run things on import here
    pass
