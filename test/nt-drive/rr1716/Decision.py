import Vision

class DecisionArg:
    closestObjFrames = [ None, #Camera 1
                         None, #Camera 2
                         None, #Camera 3
                         None  #Camera 4
                       ]
    #is the robot holding someting?
    holdingSomething = False
    #in seconds
    timeLeft = 120

    def __init__(self, objectsInFrames, holding, gameTimeLeft):
        self.closestObjFrames = objectsInFrames
        self.holdingSomething = holding
        self.timeLeft = gameTimeLeft
    
    def getClosest(self):
        return self.closestObjFrames
    def isHolding(self):
        return self.holdingSomething
    def getTimeLeft(self):
        return self.timeLeft

class DecisionOutput:
    #switch from mecanum to tank or tank to mecanum
    switchMode = False
    #speed of rotation
    driveRotation = 0
    #0 = do nothing, 1 = put down, 2 = pick up
    grabberAction = 0
    #whether the robot should start the auto balance
    startBalance = False

    def __init__(self, switchDriveMode, rotation, grabAction, balance):
        self.switchMode = switchDriveMode
        self.driveRotation = rotation
        self.grabberAction = grabAction
        self.startBalance = balance
    
    def getSwitchMode(self):
        return self.switchMode
    def getDriveRotation(self):
        return self.driveRotation
    def getGrabberAction(self):
        return self.grabberAction
    def getStartBalance(self):
        return self.startBalance

#returns a DecisionOutput object with data to be sent
#to networktables
def decision(info):

    #decision making code goes here

    return DecisionOutput(False, 0.1, 0, False)
