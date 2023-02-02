#!/usr/bin/env python

import logging
import cv2
import numpy as np

class Camera:
    def __init__(self, device):
        logging.debug("camera.init")
        self.frame = None
        self.hsv = None
        self.gray = None
        self.camera = cv2.VideoCapture(device)

    def update(self):
        logging.debug("Camera.update")
        success, frame = self.camera.read()
        if success:
            self.frame = frame
            self.hsv = None
            self.gray = None
        else:
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


if __name__ == "__main__":
    # We're a module, never run anything here
    pass
else:
    # Run things on import here
    
    pass
