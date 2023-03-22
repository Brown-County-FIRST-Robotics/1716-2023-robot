import cv2
from networktables import NetworkTables
import threading
import apriltag
import math

cond = threading.Condition()
notified = [False]

def convert(pt):
    return (int(pt[0]), int(pt[1]))

def connectionListener(connected, info):
    print(info, '; Connected=%s' % connected)
    with cond:
        notified[i] = True
        cond.notify()

def main():
    
    #Initialize networktables
    NetworkTables.initialize(server="10.17.16.2")
    #NetworkTables.addConnectionListener(connectionListener, immediateNotify=True) 
    
    #with cond:
    #    print("Waiting")
    #    if not notified[0]:
    #        cond.wait()
    print("Connected!")

    #This will be the network table that will store the position of an april tag
    #will store the largest april tag
    table = NetworkTables.getTable("apriltag") 
    table.putNumber("x", 0)
    table.putNumber("y", 0)
    table.putNumber("side", 0)

    cam = cv2.VideoCapture()
    cam.open("/dev/video0")
    cam.set(cv2.CAP_PROP_FRAME_WIDTH, 160)
    cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 120)

    while True:

        ret, frame = cam.read()

        if not ret:
            break

        #grayscale the image
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)        
        options = apriltag.DetectorOptions(families="tag36h11")
        detector = apriltag.Detector(options)
        results = detector.detect(gray)

        if len(results) > 0:
            largest = {
                "x": 0,
                "y": 0,
                "side": 0
            }

            for r in results:
                #get bounding box of apriltag
                (ptA, ptB, ptC, ptD) = r.corners
                ptA = convert(ptA)
                ptB = convert(ptB)
                ptC = convert(ptC)
                ptD = convert(ptD)

                sideLen = math.sqrt((ptA[0] - ptB[0]) * (ptA[0] - ptB[0]) +
                                    (ptA[1] - ptB[1]) * (ptA[1] - ptB[1])) 
                if sideLen > largest["side"]:
                    largest["x"] = r.center[0] 
                    largest["y"] = r.center[1]
                    largest["side"] = sideLen
                
                #For debug only
                cv2.line(frame, ptA, ptB, (0, 0, 255), 2)
                cv2.line(frame, ptB, ptC, (0, 0, 255), 2)
                cv2.line(frame, ptC, ptD, (0, 0, 255), 2)
                cv2.line(frame, ptD, ptA, (0, 0, 255), 2)
            
                (cX, cY) = (int(r.center[0]), int(r.center[1]));
                cv2.circle(frame, (cX, cY), 5, (0, 0, 255,), -1)
            
            #send results to networktables
            table.putNumber("x", largest["x"])
            table.putNumber("y", largest["y"])
            table.putNumber("side", largest["side"])
        
        #only for debug
        cv2.imshow("Camera", frame) 
        print("April tag data: "); 
        print(str(table.getNumber("x", 0))) 
        print(str(table.getNumber("y", 0)))
        print(str(table.getNumber("side", 0)))
        print("-------------------")

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

if __name__ == "__main__":
    main()
