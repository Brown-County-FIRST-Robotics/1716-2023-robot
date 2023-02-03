import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import Camera

import unittest

class TestCamera(unittest.TestCase):

    def setUp(self):
        self.cam = Camera.Camera(0)

    def tearDown(self):
        pass


    def testUpdate(self):
        self.cam.update()

    def testGetFram(self):
        self.cam.update()
        self.cam.get_frame()
        self.cam.get_frame(flipped=True)

    def testGetGray(self):
        self.cam.update()
        self.cam.get_gray()
        self.cam.get_hsv()
    
    def testGetHight(self):
        self.cam.update()
        self.cam.get_height()
        self.cam.get_width()

    

