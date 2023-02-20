#TODO: Implement a particle filter
import math
import time

def toPos(fl,fr,bl,br):
    y=fl+fr+bl+br
    x=fl+br-bl-fr
    r=fr+br-fl-bl
    return x,y,r

class Filter:
    def __init__(self):
        #self.pos=[None,None,None] # x,y,r
        self.lastApril=[None,None,None]
 #       self.lastAprilTime=0

    def updateWithApriltag(self, april, ntInterface):
        #self.pos=april.calcFieldPos()
        self.lastApril=april.calcFieldPos()
#        self.lastAprilTime=time.time()
        ntInterface.ResetEncoderVals()

    def getCurrentPos(self, ntInterface):
        fl,bl,fr,br=[i*4*math.pi for i in ntInterface.GetEncoderVals()] # fl bl fr br
        y = fl + fr + bl + br
        x = fl + br - bl - fr
        r = fr + br - fl - bl
        return self.lastApril[0] + x, self.lastApril[1] + y,self.lastApril[2] + r
