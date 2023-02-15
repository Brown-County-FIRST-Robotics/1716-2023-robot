import math

from rr1716 import Vision
from rr1716 import AprilTags
from rr1716 import Strategy
import cv2
import numpy as np
# from rr1716 import Filter


class Action:
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        self.referrer = referrer
        self.nt_interface = nt_interface
        self.cams = cams
        self.filter = filter
        self.april_executor = april_executor


    def GetFilter(self):
        robotLocation = None
        april_futures = []

        for cam in self.cams:
            april_futures.append(self.april_executor.submit(AprilTags.getPosition, cam.get_gray(), cam.camera_matrix, None))
        for future in april_futures:
            detections = future.result()
            if detections is not None:
                robotLocation=detections[0]
                break
        if robotLocation is not None:
            self.filter.updateWithApriltag(robotLocation, self.nt_interface)
        return self.filter.getCurrentPos(self.nt_interface)

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
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)

    def Step(self):
        robotLocation = None
        for camera in self.cams:
            res = AprilTags.getPosition(camera.get_gray(), camera.camera_matrix, None)  # check for apriltag
            if res is not None and res != []:  # TODO: change?
                robotLocation = res[0]  # TODO: change?
                break
        if robotLocation is not None:
            self.filter.updateWithApriltag(robotLocation, self.nt_interface)

    def ShouldEnd(self):
        return None not in self.filter.lastApril  # check if lastApril has a None, meaning it has not seen an apriltag

    def MakeChild(self):
        return AsyncSetHeight(self.filter, self.cams, self.nt_interface, self.april_executor, self.referrer, 6)  # IMPORTANT: change


class AsyncSetHeight(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer, height):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
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
        if self.height == 0:
            self.nt_interface.SetArmFloor()
        elif self.height == 1:
            self.nt_interface.SetArmPortal()
        elif self.height == 2:
            self.nt_interface.SetArmLow()
        elif self.height == 3:
            self.nt_interface.SetArmMedium()
        elif self.height == 4:
            self.nt_interface.SetArmHigh()
        elif self.height == 5:
            self.nt_interface.SetArmLowNode()
        elif self.height == 6:
            self.nt_interface.SetArmHighNode()

    def MakeChild(self):
        if self.referrer == 'auto':
            return DriveToLocation(self.filter, self.cams, self.nt_interface,self.april_executor, [0, 0, 0], self.referrer)


class DriveToLocation(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, location, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        self.location = location

    def Step(self):
        field_x, field_y, field_r = self.GetFilter()
        cx = math.cos(field_r * math.pi / 180)
        cy = math.sin(field_r * math.pi / 180)

        ax = math.cos((field_r + 90) * math.pi / 180)
        ay = math.sin((field_r + 90) * math.pi / 180)

        offset_x = field_x - self.location[0]
        offset_y = field_y - self.location[1]
        offset_r = field_r - self.location[2]

        self.nt_interface.Drive(Strategy.xy_p_factor * (offset_x * cx + offset_y * cy),
                               Strategy.xy_p_factor * (offset_x * ax + offset_y * ay), Strategy.r_p_factor * offset_r)

    def ShouldEnd(self):
        field_x, field_y, field_r = self.GetFilter()
        error = math.sqrt((field_x - self.location[0]) ** 2 + (
                    field_y - self.location[1]) ** 2) + Strategy.r_error_factor * math.fabs(field_r - self.location[2])
        return error < Strategy.drive_error_threshold

    def End(self):
        self.nt_interface.Drive(0, 0, 0)

    def MakeChild(self):
        if self.referrer == 'auto':
            return AwaitSetHeight(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer)


class AwaitSetHeight(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)

    def ShouldEnd(self):
        return self.nt_interface.IsArmDone()

    def MakeChild(self):
        if self.referrer == "auto":
            return Drop(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer)  # IMPORTANT: change


class Drop(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        self.nt_interface.DropObject()
        self.nt_interface.RetractArm()

    def MakeChild(self):
        if self.referrer == "auto":
            return GetOnStation(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer)  # IMPORTANT: change


class GetOnStation(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        self.nt_interface.SwitchToTank()
        self.nt_interface.Drive(0, -1, 0)

    def ShouldEnd(self):
        accel = self.nt_interface.GetAccel()
        return math.atan(accel[0] / accel[2]) * 180 / math.pi < Strategy.accel_angle_threshold

    def End(self):
        self.nt_interface.Drive(0, 0, 0)

    def MakeChild(self):
        return AutoBalance(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer)


class AutoBalance(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        self.nt_interface.StartAutoBalance()

    def ShouldEnd(self):
        return self.nt_interface.IsTeleop() and self.referrer=='auto'

    def End(self):
        self.nt_interface.EndAutoBalance()
        self.nt_interface.SwitchToMecanum()

    def MakeChild(self):
        pass  # if self.referrer=="auto":
        # return GetOnStation(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer)  # IMPORTANT: change


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
