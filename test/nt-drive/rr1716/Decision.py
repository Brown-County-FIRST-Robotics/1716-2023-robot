import math
import logging
import os

from rr1716 import Vision
from rr1716 import AprilTags
from rr1716 import Strategy
import cv2
import numpy as np
import simple_pid

import time
# from rr1716 import Filter
# from rr1716 import StateEstimator

class Action:
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        self.referrer = referrer
        self.nt_interface = nt_interface
        self.cams = cams
        self.filter = filter
        self.april_executor = april_executor
        self.april_cams=[]
        self.conecube_cams=[]
        for i in self.cams:
            if i.role == 'apriltag' or i.role == '*':
                self.april_cams.append(i)
            if i.role == 'conecube' or i.role == '*':
                self.conecube_cams.append(i)

    def FetchApriltags(self):
        robotLocation = None
        april_futures = []

        for cam in self.april_cams:
            april_futures.append(self.april_executor.submit(AprilTags.getPosition, cam.get_gray(), cam.camera_matrix, None))
        for future in april_futures:
            detections = future.result()
            if detections is not None:
                robotLocation=detections[0]
                break
        if robotLocation is not None:
            self.filter.updateWithApriltag(robotLocation)

    def GetFilter(self):
        logging.info(f'filter pos: {self.filter.currentTuple}')
        return self.filter.currentTuple

    def GetGameObjects(self):
        pass

    def Step(self):
        self.FetchApriltags()
        self.filter.updateWithCommandedVelocity(*self.nt_interface.GetMotors())

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
        # DO NOT ADD super().step() HERE
        robotLocation = None
        for camera in self.april_cams:
            res = AprilTags.getPosition(camera.get_gray(), camera.camera_matrix, None)  # check for apriltag
            if res is not None and res != []:  # TODO: change?
                robotLocation = res[0]  # TODO: change?
                break
        if robotLocation is not None:
            self.filter.updateWithApriltag(robotLocation)

    def ShouldEnd(self):
        return self.filter._lastApril is not None  # check if lastApril has a None, meaning it has not seen an apriltag

    def MakeChild(self):
        if self.referrer=='auto':
            return AsyncSetHeight(self.filter, self.cams, self.nt_interface, self.april_executor, self.referrer, 6)  # IMPORTANT: change
        elif self.referrer=='DRIVETOAPRILTAG':
            return DriveToLocation(self.filter, self.cams, self.nt_interface, self.april_executor, (600,-400,0), self.referrer)


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
            return DriveToLocation(self.filter, self.cams, self.nt_interface,self.april_executor, [500, -293, 0], self.referrer)


class DriveToLocation(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, location, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        self.location = location
        self.xy_pid = simple_pid.PID(*Strategy.xy_pid_factor)
        self.r_pid = simple_pid.PID(*Strategy.r_pid_factor)

    def Step(self):
        super().Step()
        state = self.filter.current
        field_x, field_y, field_r = state.x, state.y, state.theta
        field_x-=self.location[0]
        field_y-=self.location[1]
        logging.info(f'Moves:{field_x} {field_y} {field_r}')
        theta  = field_r-self.location[2]
        if theta>180:
            theta-=360
        fxfromrx = math.cos(field_r * math.pi / 180)#1 0
        fyfromrx = math.sin(field_r * math.pi / 180)#0 -1

        fxfromry = math.cos((field_r + 90) * math.pi / 180)#0 1
        fyfromry = math.sin((field_r + 90) * math.pi / 180)#1 0

        move_y = field_x * fxfromrx + field_y * fxfromry
        move_x = field_x * fyfromrx + field_y * fyfromry

        self.nt_interface.Drive(Strategy.xy_pid_factor[0]*(move_x),-Strategy.xy_pid_factor[0]*(move_y),Strategy.r_pid_factor[0]*(theta))

    def ShouldEnd(self):
        state=self.filter.current
        field_x, field_y, field_r = state.x, state.y, state.theta
        error = math.sqrt((field_x - self.location[0]) ** 2 + (
                    field_y - self.location[1]) ** 2)
        return error < Strategy.drive_error_threshold and math.fabs(field_r - self.location[2]) < Strategy.r_error_threshold

    def End(self):
        self.nt_interface.Drive(0, 0, 0)

    def MakeChild(self):
        if self.referrer == 'auto':
            return AwaitSetHeight(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer)
        assert 0


class AwaitSetHeight(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)

    def ShouldEnd(self):
        return self.nt_interface.IsArmDone()

    def MakeChild(self):
        if self.referrer == 'auto':
            return Drop(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer)  # IMPORTANT: change


class DriveDumb(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, location, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        self.location = location
        self.xy_pid = simple_pid.PID(*Strategy.xy_pid_factor)
        self.r_pid = simple_pid.PID(*Strategy.r_pid_factor)

    def Step(self):
        cam=self.april_cams[0]
        dets=AprilTags.getPosition(cam.get_gray(), cam.camera_matrix, None)
        if len(dets)==0:
            return
        det=dets[0]

        offset_y = float(det.distance-100)
        offset_x = float(det.left_right)
        offset_r = float(det.yaw)

        self.nt_interface.Drive(offset_x*0.01,
                               offset_y*0.01, offset_r*0.0025)
        #self.nt_interface.Drive(1,0,0)


    def ShouldEnd(self):
        return False

class Drop(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        self.nt_interface.DropObject()
        self.nt_interface.RetractArm()

    def MakeChild(self):
        if self.referrer == 'auto':
            return GetOnStation(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer)  # IMPORTANT: change


class GetOnStation(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        self.nt_interface.SwitchToTank()
        self.nt_interface.Drive(0, -1, 0)

    def Step(self):
        # DO NOT ADD super().step() HERE
        pass

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

    def Step(self):
        # DO NOT ADD super().step() HERE
        pass

    def ShouldEnd(self):
        return self.nt_interface.IsTeleop() and self.referrer=='auto'

    def End(self):
        self.nt_interface.EndAutoBalance()
        self.nt_interface.SwitchToMecanum()

    def MakeChild(self):
        return GetDriverCommand(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer)

def convert(inp):
    return [int(i) for i in inp]

class AddScreenVals(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        col = []
        if os.path.exists("cube_picked_color"):
            with open("cube_picked_color", "r") as file:
                for line in file:
                    for x in line.split():
                        col.append(int(x))

        self.cone = Vision.GamePiece()
        while len(col) < 3:
            col.append(0)

        lower = [col[0] * 0.5, col[1] * 0.5, col[2] * 0.1]
        upper = [col[0] * 1.4, col[1] * 1.4, col[2] * 4.0]

        for i in range(len(lower)):
            if lower[i] < 0:
                lower[i] = 0
            if lower[i] > 255:
                lower[i] = 255

            if upper[i] < 0:
                upper[i] = 0
            if upper[i] > 255:
                upper[i] = 255

        self.cone.setLowerColor(np.array(lower, dtype=np.uint8))
        self.cone.setUpperColor(np.array(upper, dtype=np.uint8))

    def Step(self):
        april_futures = []
        for cam in self.cams:
            april_futures.append(self.april_executor.submit(AprilTags.getCoords, cam.get_gray()))
        for cam, future in zip(self.cams,april_futures):
            detections = future.result()
            for detection in detections:
                cam.add_rectangle(convert(detection[0]),convert(detection[2]), [0,0,255])
            self.cone.findCone(cam.frame)
            print(self.cone.notfound)
            cam.frame=self.cone.drawBoundRect(cam.frame, [255,0,0])
            self.cone.findCube(cam.frame)
            cam.frame = self.cone.drawBoundRect(cam.frame, [0, 255, 0])

    def ShouldEnd(self):
        return False

class GetDriverCommand(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)

    def ShouldEnd(self):
        place = self.nt_interface.GetPlacement()
        if place is not None:
            self.referrer=f'dropoff_{place}'
            return True
        pickup=self.nt_interface.GetPickup()
        if pickup is not None:
            self.referrer=f'pickup_{pickup}'
            return True
        return False

    def End(self):
        self.nt_interface.ResetPlacement()
        self.nt_interface.ResetPickp()

    def MakeChild(self):
        if self.referrer[:7]=='dropoff':
            armHeights={1:6,2:4,3:6,4:6,5:4,6:6,7:6,8:4,9:6,10: 5,11: 3,12: 5,13: 5,14: 3,15: 5,16: 5,17: 3,18: 5,19: 0,20: 0,21: 0,22: 0,23: 0,24: 0,25: 0,26: 0,27: 0}
            return AsyncSetHeight(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer, armHeights[int(self.referrer.split('_')[-1])])
        if self.referrer[:6]=='pickup':
            return AsyncSetHeight(self.filter, self.cams, self.nt_interface,self.april_executor, self.referrer, 1)

def doCurrentAction(action):
    action.Step()
    if action.ShouldEnd():
        action.End()
        return action.MakeChild()
    return None

class DriveToGamepeice(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer, col_range_h=50, col_range_s=50, col_range_v=50, target_w=300, target_h=200, color_file_path="cone_picked_color", 
                 minRatio=-1, maxRatio=99999):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
       
        self.minRatio = minRatio
        self.maxRatio = maxRatio

        col = []
        if os.path.exists(color_file_path):
            file = open(color_file_path, "r") 
            for line in file:
                for x in line.split():
                    col.append(int(x)) 
            file.close()
        else:
            print("Error: file does not exist: " + color_file_path)

        self.gamepeice = Vision.GamePiece()
        while len(col) < 3:
            col.append(0)
        
        lower = [col[0] - col_range_h, col[1] - col_range_s, col[2] - col_range_v]
        upper = [col[0] + col_range_h, col[1] + col_range_s, col[2] + col_range_v]
    
        for i in range(len(lower)):
            if lower[i] < 0:
                lower[i] = 0
            if lower[i] > 255:
                lower[i] = 255

            if upper[i] < 0:
                upper[i] = 0
            if upper[i] > 255:
                upper[i] = 255

        self.gamepeice.setLowerColor(np.array(lower, dtype=np.uint8))
        self.gamepeice.setUpperColor(np.array(upper, dtype=np.uint8))
        self.gamepeice.setMinRatio(self.minRatio)
        self.gamepeice.setMaxRatio(self.maxRatio)

        self.target_w = target_w
        self.target_h = target_h

    def Step(self):
        x = y = r = 0
        self.gamepeice.findObject(self.conecube_cams[0].frame) #find the cone
       
        # perfect, do nothing!
        if (self.gamepeice.w >= self.target_w or self.gamepeice.h >= self.target_h) and self.gamepeice.x >= -5 - self.gamepeice.w / 2 and self.gamepeice.x <= 5 + self.gamepeice.w / 2:
            self.nt_interface.Drive(0, 0, 0)
            return

        logging.debug("Decision: w: " + str(self.gamepeice.w))
        logging.debug("Decision: x, y: " + str(self.gamepeice.x) + ", " + str(self.gamepeice.y))

        # cone is to the left, turn left
        if self.gamepeice.x < -5 - self.gamepeice.w / 2:
            print("turn left")
            r = -0.05
        # cone is to the right, turn right
        elif self.gamepeice.x > 5 + self.gamepeice.w / 2:
            print("turn right")
            r = 0.05

        # too far away, drive towards it
        if self.gamepeice.w < self.target_w and self.gamepeice.h < self.target_h:
            print("drive forward")
            y = 0.4 - 0.3 * self.gamepeice.w / self.target_w * 0.3 

        self.nt_interface.Drive(x, y, r)

    def ShouldEnd(self):
        if (self.gamepeice.w >= self.target_w or self.gamepeice.h >= self.target_h) and self.gamepeice.x >= -5 - self.gamepeice.w / 2 and self.gamepeice.x <= 5 + self.gamepeice.w / 2:
            return True
        return False

    def End(self):
        self.nt_interface.Drive(0, 0, 0)
        
    def MakeChild(self):
        if self.referrer == "auto":
            return AutoTurn180(self.filter, self.cams, self.nt_interface, self.april_executor, "drivetogamepeice")
        return None

class AutoTurn180(Action):
    def __init__(self, filter, cams, nt_interface, april_executor, referrer):
        super().__init__(filter, cams, nt_interface, april_executor, referrer)
        self.startrotation = self.nt_interface.GetYaw()
        if self.startrotation == None:
            self.startrotation = 0

    def Step(self):
        logging.info("rotating")
        # just turn until we are at 180 degrees
        self.nt_interface.Drive(0, 0, 0.2)

    def ShouldEnd(self):
        if self.nt_interface.GetYaw() == None:
            return False
        target_rotation = self.startrotation + 180.0
        # put target_rotation in the range 0 to 360
        while target_rotation >= 360.0:
            target_rotation -= 360.0
        while target_rotation < 0.0:
            target_rotation += 360.0
        return math.fabs(self.nt_interface.GetYaw() - target_rotation) < 20.0
    
    def End(self):
        self.nt_interface.Drive(0, 0, 0)

    def MakeChild(self):
        if self.referrer == "auto":
            logging.info("switch to drive to gamepeice")
            return DriveToGamepeice(self.filter, self.cams, self.nt_interface, self.april_executor, self.referrer, 5, 100, 100, Strategy.TARGET_CUBE_SIZE, Strategy.TARGET_CUBE_SIZE, "cube_picked_color", 5.0 / 4.0, 5.0 / 3.0)

        elif self.referrer == "drivetogamepeice":  
            logging.info("switch to drive to april tag")
            return DriveDumb(self.filter, self.cams, self.nt_interface, self.april_executor, None, self.referrer) 
        return None 

# TEST CODE GOES HERE
if __name__ == '__main__':
    pass
# initialize module here
else:
    pass
