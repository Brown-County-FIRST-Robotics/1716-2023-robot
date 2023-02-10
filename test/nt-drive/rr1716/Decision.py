import Vision


class DecisionArg:
    closestObjFrames = [None,  # Camera 1
                        None,  # Camera 2
                        None,  # Camera 3
                        None  # Camera 4
                        ]
    # is the robot holding something?
    holdingSomething = False
    # in seconds
    timeLeft = 120

    def __init__(self, objects_in_frames, holding, game_time_left):
        self.closestObjFrames = objects_in_frames
        self.holdingSomething = holding
        self.timeLeft = game_time_left

    def getClosest(self):
        return self.closestObjFrames

    def isHolding(self):
        return self.holdingSomething

    def getTimeLeft(self):
        return self.timeLeft


class DecisionOutput:
    # switch from mecanum to tank or tank to mecanum
    switchMode = False
    # speed of rotation
    driveRotation = 0
    driveSpeed = 0
    # 0 = do nothing, 1 = put down, 2 = pick up
    grabberAction = 0
    # whether the robot should start the auto balance
    startBalance = False

    def __init__(self, switch_drive_mode, rotation, speed, grab_action, balance):
        self.switchMode = switch_drive_mode
        self.driveRotation = rotation
        self.driveSpeed = speed
        self.grabberAction = grab_action
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


# returns a DecisionOutput object with data to be sent
# to networktables
def decision(info):
    closest = info.closestObjFrames
    # Attempt to center the object in the front camera
    if not info.isHolding():
        for i in range(len(closest)):
            obj = closest[i]

            if obj.notfound:
                continue

            # too close!
            if obj.w > 100 or obj.h > 400:
                return DecisionOutput(False, 0.0, 0.0, 0, False)

            if obj.x < -20:
                return DecisionOutput(False, -0.1, 0.1, 0, False)
            elif obj.x > 20:
                return DecisionOutput(False, 0.1, 0.1, 0, False)
            else:
                return DecisionOutput(False, 0, 0, 0, False)

    return DecisionOutput(False, 0.0, 0.0, 0, False)


# TEST CODE GOES HERE
if __name__ == "__main__":
    import NetworkTables1716
    import cv2
    import numpy as np

    gameobjects = [
        Vision.GamePiece(),
        Vision.GamePiece(),
        Vision.GamePiece(),
        Vision.GamePiece()
    ]

    gameobjects[0].x = -30
    gameobjects[0].y = 0

    gameobjects[1].notfound = True
    gameobjects[2].notfound = True
    gameobjects[3].notfound = True

    nttable = NetworkTables1716.NetworkTablesWrapper()
    cam = cv2.VideoCapture("/dev/video0")

    ret, frame = cam.read()
    avgColor = Vision.averageColor(frame, 100)

    low = [avgColor[0] * 0.3, avgColor[1] * 0.7, avgColor[2] * 0.7]
    high = [avgColor[0] * 2.0, avgColor[1] * 2.0, avgColor[2] * 2.0]
    gameobjects[0].setLowerColor(np.array(low, dtype=np.uint8))
    gameobjects[0].setUpperColor(np.array(high, dtype=np.uint8))

    while True:
        ret, frame = cam.read()

        gameobjects[0].findCone(frame)
        gameobjects[0].drawBoundRect(frame, [0, 255, 0])

        print(int(gameobjects[0].x), int(gameobjects[0].y))

        scaled_up = cv2.resize(frame, (int(frame.shape[1] * 1.6), int(frame.shape[0] * 1.6)),
                               interpolation=cv2.INTER_AREA)
        cv2.imshow("frame", scaled_up)

        decisionMade = decision(DecisionArg(gameobjects, False, 120))
        print("rotation speed:", decisionMade.driveRotation, "speed:", decisionMade.driveSpeed)
        nttable.Drive(decisionMade.driveSpeed, 0.0, decisionMade.driveRotation)

        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            break
        elif key == ord('1'):
            avgColor = Vision.averageColor(frame, 100)
            low = [avgColor[0] * 0.3, avgColor[1] * 0.5, avgColor[2] * 0.5]
            high = [avgColor[0] * 2.0, avgColor[1] * 2.0, avgColor[2] * 2.0]

            for i in range(3):
                if low[i] < 0:
                    low[i] = 0
                elif low[i] > 255:
                    low[i] = 255

                if high[i] < 0:
                    high[i] = 0
                elif high[i] > 255:
                    high[i] = 255

            print(low, high, avgColor)
            gameobjects[0].setLowerColor(np.array(low, dtype=np.uint8))
            gameobjects[0].setUpperColor(np.array(high, dtype=np.uint8))

    pass
# initialize module here
else:
    pass
