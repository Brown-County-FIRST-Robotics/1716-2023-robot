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
    driveSpeed = 0
    #0 = do nothing, 1 = put down, 2 = pick up
    grabberAction = 0
    #whether the robot should start the auto balance
    startBalance = False

    def __init__(self, switchDriveMode, rotation, speed, grabAction, balance):
        self.switchMode = switchDriveMode
        self.driveRotation = rotation
        self.driveSpeed = speed
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
    def getDriveSpeed(self):
        return self.driveSpeed

#returns a DecisionOutput object with data to be sent
#to networktables
def decision(info):
    closest = info.getClosest()

    #Attempt to center the object in the front camera
    if not info.isHolding():
        for i in range(len(closest)):
            obj = closest[i]

            if obj.notfound:
                continue

            if(obj.x < -20): 
                return DecisionOutput(False, -0.1, 0.1, 0, False)
            elif(obj.x > 20): 
                return DecisionOutput(False, 0.1, 0.1, 0, False)

    return DecisionOutput(False, 0.0, 0.0, 0, False)

#TEST CODE GOES HERE
if __name__ == "__main__":
    gameobjects = [
        Vision.GamePiece(),
        Vision.GamePiece(),
        Vision.GamePiece(),
        Vision.GamePiece()
    ]

    gameobjects[0].x = -40
    gameobjects[0].y = 0

    gameobjects[1].notfound = True 
    gameobjects[2].notfound = True
    gameobjects[3].notfound = True

    decisionMade = decision(DecisionArg(gameobjects, False, 120))
    print("rotation speed:", decisionMade.driveRotation, "speed:", decisionMade.driveSpeed) 

    pass
#initialize module here
else:
    pass
