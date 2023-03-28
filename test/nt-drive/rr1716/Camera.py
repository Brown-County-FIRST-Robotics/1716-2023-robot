#!/usr/bin/env python

import logging
import cv2
import time
import threading
import numpy as np

class Camera:
    def __init__(self, device, calibration, position, role):
        logging.debug("camera init %s", device)
        self.frame = None
        self.hsv = None
        self.gray = None

        self.id = None
        self.frame_count = 0
        self.last_frame_count = 0
        self.device = device
        self.camera = cv2.VideoCapture(device)
        self.role=role

        self.rectangles = None
        assert self.camera.isOpened()
        self.pos=position
        if calibration is not None:
            video_size = tuple(calibration["calibrationResolution"])
            self.camera.set(cv2.CAP_PROP_FRAME_WIDTH, video_size[0])
            self.camera.set(cv2.CAP_PROP_FRAME_HEIGHT, video_size[1])
            test_video_size = (self.camera.get(cv2.CAP_PROP_FRAME_WIDTH), self.camera.get(cv2.CAP_PROP_FRAME_HEIGHT))
            assert tuple(test_video_size) == tuple(video_size), 'camera resolution didnt set'

            self.camera.set(cv2.CAP_PROP_BUFFERSIZE, 1)

            raw_camera_matrix = np.array(calibration['cameraMatrix'])
            dist_coefficients = np.array(calibration['cameraDistortion'])
            processing_resolution = np.array(calibration['processingResolution'])

            self.camera_matrix, roi = cv2.getOptimalNewCameraMatrix(raw_camera_matrix, dist_coefficients, tuple(video_size), 0,tuple(processing_resolution))
            self.map1, self.map2 = cv2.initUndistortRectifyMap(raw_camera_matrix, dist_coefficients, None, self.camera_matrix,
                                                     tuple(processing_resolution), cv2.CV_16SC2)
        else:
            assert role != 'apriltag' and role != '*', f'For the role to be "{role}", a calibration is required'
            self.camera_matrix=None

        self.camera.set(cv2.CAP_PROP_FPS, 10)
        self.camera.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))

        self.camera.set(cv2.CAP_PROP_BRIGHTNESS, 0)
        self.camera.set(cv2.CAP_PROP_CONTRAST, 0)
        self.camera.set(cv2.CAP_PROP_SATURATION, 128)
        self.camera.set(cv2.CAP_PROP_HUE, 0)
        self.camera.set(cv2.CAP_PROP_AUTO_WB, 1)
        self.camera.set(cv2.CAP_PROP_GAMMA, 100)
        self.camera.set(cv2.CAP_PROP_GAIN, 0)
        self.camera.set(cv2.CAP_PROP_WB_TEMPERATURE, 3200)
        self.camera.set(cv2.CAP_PROP_SHARPNESS, 2)

        self._stopping = False
        self._getframe = threading.Event()
        self._frameready = threading.Event()
        self.thread = threading.Thread(target=self._thread)
        self.thread.start()
        self.update()
        if self.frame is None:
            self.shutdown()
            raise Exception('camera intialization failed on ' + device)
        

    def _thread(self):
        while not self._stopping:
            success, frame = self.camera.read()
            if success and frame is not None:
                if self.camera_matrix is not None:
                    frame = cv2.remap(frame, self.map1, self.map2, cv2.INTER_CUBIC)
                self.frame = frame
                self.frame_count = self.frame_count + 1
            else:
                logging.error("camera frame cap failed %s", self.device)
                self.frame=None
            self.hsv = None
            self.gray = None

            self._getframe.clear()
            self._frameready.set()
            self._getframe.wait()


    def shutdown(self):
        self._stopping = True
        self._getframe.set()
        self.thread.join()


    def startupdate(self):
        logging.debug("Camera update start")
        if self._getframe.is_set():
          self._frameready.wait()
        self._frameready.clear()
        self._getframe.set()

    def update(self):
        self.startupdate()
        self._frameready.wait()
        if self.frame is None:
            logging.critical("Camera Read Failed")

    
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
        if self.frame is None:
            return None
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
        self.frame = cv2.rectangle(self.frame, start, end, color, thickness)

    def clear_rectangle(self):
        logging.debug("camera.clear_rectangle")
        self.rectangles = None

    def get_jpg_bytes(self, flipped=False):
        # Let's block on this call if we alredy returned this frame
        while self.frame_count <= self.last_frame_count:
            self._getframe.wait()
            self.wait()
        self.last_frame_count = self.frame_count
        logging.debug("camera.get_jpg_bytes")
        frame = self.get_frame(flipped)
        
        if self.rectangles:
            cv2.rectangle(self.frame, self.rectangles[0], self.rectangles[1], (0, 255, 0), self.rectangles[2])

        ret, buffer = cv2.imencode('.jpg', frame)
        jpg = buffer.tobytes()
        return jpg

    def wait(self):
       self._frameready.wait()


if __name__ == "__main__":
    try:
        openCameras()        
        print('started successfully')
    finally:
        for camera in Cameras:
            camera.shutdown()
            
    print('done')
    
