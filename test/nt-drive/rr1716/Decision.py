import math
import logging
import os

import rr1716
from rr1716 import Vision
from rr1716 import AprilTags
from rr1716 import Strategy
import cv2
import numpy as np
import simple_pid

import time


class BaseAction:  # TODO: consider renaming
    def __init__(self, filter, cams, april_executor, nt_interface):
        self.filter = filter
        self.cams: list[rr1716.Camera] = cams
        self.april_executor = april_executor
        self.nt_interface = nt_interface

        self.april_cams = []
        self.conecube_cams = []
        for i in self.cams:
            if i.role == 'apriltag' or i.role == '*':
                self.april_cams.append(i)
            if i.role == 'conecube' or i.role == '*':
                self.conecube_cams.append(i)

    def getFilter(self):
        logging.info(f'filter pos: {self.filter.currentTuple}')
        return self.filter.currentTuple

    # TODO: Add fetchApriltag and fetchConecube

    def start(self):
        pass

    def step(self):
        pass

    def shouldEnd(self):
        return True

    def end(self):
        pass


def doCurrentAction(action):
    action.Step()
    if action.ShouldEnd():
        action.End()
        return action.MakeChild()
    return None


# TEST CODE GOES HERE
if __name__ == '__main__':
    pass
# initialize module here
else:
    pass
