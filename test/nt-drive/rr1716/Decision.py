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
        pass

    def GetGameObjects(self):
        pass

    def Execute(self):
        pass

    def ShouldEnd(self):
        pass

    def End(self):
        pass

    def MakeChild(self):
        pass


class PutDown(Action):
    def __init__(self, filter, cams, ntInterface):
        super().__init__(filter, cams, ntInterface)


def doCurrentAction(action):
    action.Execute()
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
