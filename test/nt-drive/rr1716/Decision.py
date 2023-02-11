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
        pass

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
        return Dri(self.filter, self.cams, self.ntInterface)

class Dri(Action):
    def __init__(self, filter, cams, ntInterface):
        super().__init__(filter, cams, ntInterface)
        print('dri init')


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
