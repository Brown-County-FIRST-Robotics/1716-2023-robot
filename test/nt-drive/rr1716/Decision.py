import math

from rr1716 import Vision
from rr1716 import AprilTags
import cv2
import numpy as np
#from rr1716 import Filter

class Action:
    def __init__(self, filter, cams, ntInterface):
        self.ntInterface = ntInterface
        self.cams = cams
        self.filter = filter

    def GetFilter(self):
        robotLocation=None
        for camera in self.cams:
            res = AprilTags.getPosition(camera.get_gray(), camera.camera_matrix, None) # check for apriltag
            if res is not None and res!=[]: # TODO: change?
                robotLocation=res[0] # TODO: change?
                break
        if robotLocation is not None:
            self.filter.updateWithApriltag(robotLocation, self.ntInterface)
        return self.filter.getCurrentPos(self.ntInterface)

    def GetGameObjects(self):
        pass

    def Step(self):
        pass

    def ShouldEnd(self):
        return True

    def End(self):
        pass

    def MakeChild(self):
        pass


class StartFilter(Action):
    def __init__(self, filter, cams, ntInterface):
        super().__init__(filter, cams, ntInterface)

    def Step(self):
        robotLocation = None
        for camera in self.cams:
            res = AprilTags.getPosition(camera.get_gray(), camera.camera_matrix, None)  # check for apriltag
            if res is not None and res != []:  # TODO: change?
                robotLocation = res[0]  # TODO: change?
                break
        if robotLocation is not None:
            self.filter.updateWithApriltag(robotLocation, self.ntInterface)

    def ShouldEnd(self):
        return None not in self.filter.lastApril # check if lastApril has a None, meaning it has not seen an apriltag

    def MakeChild(self):
        return DriveToLocation(self.filter, self.cams, self.ntInterface, [0,0,0]) # IMPORTANT: change

class DriveToLocation(Action):
    def __init__(self, filter, cams, ntInterface, location):
        super().__init__(filter, cams, ntInterface)
        self.location = location

    def Step(self):
        field_x,field_y,field_r=self.filter.getCurrentPos(self.ntInterface)
        cx = math.cos((field_r)*math.pi/180)
        cy = math.sin((field_r) * math.pi / 180)

        ax = math.cos((field_r+90) * math.pi / 180)
        ay = math.sin((field_r+90) * math.pi / 180)

        offset_x=field_x-self.location[0]
        offset_y = field_y - self.location[1]
        offset_r = field_r - self.location[2]

        self.ntInterface.Drive(0.1*(offset_x*cx+offset_y*cy),0.1*(offset_x*ax+offset_y*ay),0.1*offset_r)
        
    def ShouldEnd(self):
        return False


def doCurrentAction(action):
    action.Step()
    if action.ShouldEnd():
        action.End()
        return action.MakeChild()
    return None


# TEST CODE GOES HERE
if __name__ == "__main__":
   pass
# initialize module here
else:
    pass
