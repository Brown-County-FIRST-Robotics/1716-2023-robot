# TODO: Implement a particle filter
import math
import time
import dataclasses
from rr1716 import AprilTags

# MECANUM ONLY
# TODO: add tank

@dataclasses.dataclass
class State:
    x: float
    y: float
    theta: float
    vx: float
    vy: float
    omega: float
    time: float




class StateEstimator:
    def __init__(self):
        self._current = State(0, 0, 0, 0, 0, 0, self.now())
        self._lastApril: Tuple[float, float, float, float] = None

    def now(self):
        return time.monotonic()

    def updateWithApriltag(self, april: AprilTags.Detection):
        x, y, theta = april.calcFieldPos()
        self.updateToNow()
        self._current.theta = theta
        self._current.x = x
        self._current.y = y

        #try to get some velocity info from multiple quick tag sightings
        if self._lastApril:
            update_weight = .5
            x,y,r,t = self._lastApril
            interval = self._current.time - t
            if interval < .35 and interval > .1: #should be ok with dump lower limit since fps is capped at 10.  otherwise have to save april tags to look back on
                #deal with angle wrapping around
                dTheta = (r - self._current.theta) %360
                if dTheta > 180:
                    dTheta = 360 - dTheta
                omega = dTheta / interval

                vx = (x - self._current.x) / interval
                vy = (y - self._current.y) / interval

                self._current.omega = omega * update_weight + self._current.omega * (1-update_weight)
                self._current.vx = vx * update_weight + self._current.vx * (1-update_weight)
                self._current.vy = vy * update_weight + self._current.vy * (1-update_weight)

        self._lastApril = x,y,theta,self._current.time


    @property
    def current(self):
        self.updateToNow()
        return dataclasses.replace(self._current)

    def updateToNow(self): # Update the x,y,r values, according to vx,vy,vr
        newTime = self.now()
        time_step = newTime - self._current.time
        self._current.theta = (self._current.theta + self._current.omega * time_step) % 360
        self._current.x += self._current.vx * time_step
        self._current.y += self._current.vy * time_step
        self._current.time = newTime

    def updateWithCommandedPosition(self, x, y, theta):
        self.updateToNow()
        self._current.theta = theta
        self._current.x = x
        self._current.y = y


    def updateWithCommandedVelocity(self, vx, vy, omega):
        # robot 0 theta means pointed up on field (to long wall) and +vy is forward.
        self.updateToNow()
#th0 y->y
#th270 y->x

        theta  = self._current.theta
        fxfromrx = math.cos(theta * math.pi / 180)#1 0
        fyfromrx = math.sin(theta * math.pi / 180)#0 -1

        fxfromry = math.cos((theta + 90) * math.pi / 180)#0 1
        fyfromry = math.sin((theta + 90) * math.pi / 180)#1 0

        fx = vx * fxfromrx + vy * fxfromry
        fy = vx * fyfromrx + vy * fyfromry


        self._current.omega = omega
        self._current.vx = fx
        self._current.vy = fy

