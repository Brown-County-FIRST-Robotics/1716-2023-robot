import math

from rr1716 import Vision
from rr1716 import AprilTags
from rr1716 import Strategy
import cv2
import numpy as np
#from rr1716 import Filter

class Action:
    def __init__(self, filter, cams, ntInterface, referrer):
        self.referrer = referrer
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
    def __init__(self, filter, cams, ntInterface, referrer):
        super().__init__(filter, cams, ntInterface, referrer)

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
        return AsyncSetHeight(self.filter, self.cams, self.ntInterface, 6 ,self.referrer) # IMPORTANT: change

class AsyncSetHeight(Action):
    def __init__(self, filter, cams, ntInterface, referrer, height):
        super().__init__(filter, cams, ntInterface, referrer)
        '''
        0:Floor
        1:portal
        2:Low platform
        3:Medium platform
        4:High platform
        5:Low cone node
        6:High cone node
        '''
        self.height = height
        if self.height==0:
            self.ntInterface.SetArmFloor()
        elif self.height==1:
            self.ntInterface.SetArmPortal()
        elif self.height==2:
            self.ntInterface.SetArmLow()
        elif self.height==3:
            self.ntInterface.SetArmMedium()
        elif self.height==4:
            self.ntInterface.SetArmHigh()
        elif self.height==5:
            self.ntInterface.SetArmLowNode()
        elif self.height==6:
            self.ntInterface.SetArmHighNode()

    def MakeChild(self):
        if self.referrer=='auto':
           return DriveToLocation(self.filter, self.cams, self.ntInterface, [0, 0, 0], self.referrer)


class DriveToLocation(Action):
    def __init__(self, filter, cams, ntInterface, location, referrer):
        super().__init__(filter, cams, ntInterface, referrer)
        self.location = location

    def Step(self):
        field_x,field_y,field_r=self.GetFilter()
        cx = math.cos((field_r) * math.pi / 180)
        cy = math.sin((field_r) * math.pi / 180)

        ax = math.cos((field_r+90) * math.pi / 180)
        ay = math.sin((field_r+90) * math.pi / 180)

        offset_x = field_x - self.location[0]
        offset_y = field_y - self.location[1]
        offset_r = field_r - self.location[2]

        self.ntInterface.Drive(Strategy.xy_p_factor*(offset_x*cx+offset_y*cy),Strategy.xy_p_factor*(offset_x*ax+offset_y*ay),Strategy.r_p_factor*offset_r)
        
    def ShouldEnd(self):
        field_x,field_y,field_r=self.GetFilter()
        error=math.sqrt((field_x - self.location[0])**2+(field_y - self.location[1])**2)+Strategy.r_error_factor*math.fabs(field_r-self.location[2])
        return error<Strategy.drive_error_threshold
    
    def MakeChild(self):
        if self.referrer=='auto':
            return AwaitSetHeight(self.filter, self.cams, self.ntInterface, self.referrer)

class AwaitSetHeight(Action):
    def __init__(self, filter, cams, ntInterface, referrer):
        super().__init__(filter, cams, ntInterface, referrer)

    def ShouldEnd(self):
        return self.ntInterface.IsArmDone()

    def MakeChild(self):
        pass#return DriveToLocation(self.filter, self.cams, self.ntInterface, [0, 0, 0], self.referrer)  # IMPORTANT: change


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