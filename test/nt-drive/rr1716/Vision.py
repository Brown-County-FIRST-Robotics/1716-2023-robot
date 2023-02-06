#!/usr/bin/env python

import logging
import numpy as np
import cv2

#use this for calibrating the color detector
#pass in frame
#pass in range from center of image that you want to sample
#returns average color is an array
def averageColor(frame, sampleRange):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    height = len(hsv)
    width = len(hsv[0])
    x = int(width / 2) - sampleRange
    y = int(height / 2) - sampleRange
    hSum = 0
    sSum = 0
    vSum = 0
    for i in range(x, x + 2 * sampleRange):
        for j in range(y, y + 2 * sampleRange):
            hSum += hsv[j][i][0]
            sSum += hsv[j][i][1]
            vSum += hsv[j][i][2]
    averageColor = [ hSum / (4 * sampleRange * sampleRange), sSum / (4 * sampleRange * sampleRange), vSum / (4 * sampleRange * sampleRange) ]
    return averageColor

#GamePiece - can represent a cone or a cube
class GamePiece():
    # position of cone in the image in terms of pixels
    x = 0
    y = 0
    #dimensions of bounding rectangle
    w = 0
    h = 0
    imgX = 0
    imgY = 0
    # is the cone upright?
    upright = False #Do not care about this value if this is a cube
    lower_color = np.array([ 0, 0, 0 ], np.uint8)
    upper_color = np.array([ 0xFF, 0xFF, 0xFF ], np.uint8)
    notfound = False

    def setLowerColor(self, lower):
        self.lower_color = lower
    
    def setUpperColor(self, upper):
        self.upper_color = upper

    def getLowerColor(self):
        return self.lower_color

    def getUpperColor(self):
        return self.upper_color

    def isUpright(self):
        return self.upright

    def setUpright(self, up):
        self.upright = up

    def setX(self, xpos):
        self.x = xpos

    def setY(self, ypos):
        self.y = ypos

    def getX(self):
        return self.x
    
    def getY(self):
        return self.y

    def getWidth(self):
        return self.w

    def setWidth(self, width):
        self.w = width

    def getHeight(self):
        return self.h

    def setHeight(self, height):
        self.h = height

    def getNotFound(self):
        return self.notfound
    
    def setNotFound(self, isNotFound):
        self.notfound = isNotFound

    # Returns a cone object when it attempts to find a
    # cone in an image (frame)
    # also passes in the lower color of the cone and upper color of the cone
    def findCone(self, frame):
        logging.info("looking for cone")
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
        maskKone = cv2.inRange(hsv, self.lower_color, self.upper_color)
            
        gray = cv2.cvtColor(hsv, cv2.COLOR_BGR2GRAY)
    
        contours, hierarchy = cv2.findContours(gray, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
        maskKone_copy = maskKone.copy()
    
        contours, hierarchy = cv2.findContours(image=maskKone, mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_NONE)
        largestCont = 0
        largestBoundRectArea = 0
        x = y = w = h = 0
        lx = ly = lw = lh = 0

        if len(contours) == 0:
            self.notfound = True
            return
        self.notfound = False

        #find largest contour
        for cont in contours:
            x,y,w,h = cv2.boundingRect(cont)
            area = w * h
            if area > largestBoundRectArea:
                largestBoundRectArea = area
                largestCont = cont
                lx = x
                ly = y
                lw = w
                lh = h
        
        # Determine if the cone is upright
        upright = False
        #check if the rectangle's dimensions are in
        if(lw < lh):
            yellowTop = 0
            yellowBot = 0
            for x in range(int(lx), int(lx + lw)):
                for y in range(int(ly), int(ly + lh / 2)):
                    if maskKone[y][x]:
                        yellowTop += 1
            for x in range(int(lx), int(lx + lw)):
                for y in range(int(ly + lh / 2), int(ly + lh)):
                    if maskKone[y][x]:
                        yellowBot += 1
            if yellowTop < yellowBot: 
                upright = True

        frameDimensions = frame.shape
        self.imgX = int(lx + lw / 2)
        self.imgY = int(ly + lh / 2)
        self.setX(int(lx + lw / 2) - int(frameDimensions[1] / 2))
        self.setY(int(ly + lh / 2) - int(frameDimensions[0] / 2))
        self.setHeight(int(lh))
        self.setWidth(int(lw))
        self.setUpright(upright)
    
    # Returns a cube object when it attempts to find a
    # cube in an image (frame)
    # also passes in the lower color of the cube and upper color of the cube 
    # NOTE: this also does a ratio check to roughly guess if it is a cube
    def findCube(self, frame):
        logging.info("looking for cube")
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
        maskKube = cv2.inRange(hsv, self.lower_color, self.upper_color)
             
        gray = cv2.cvtColor(hsv, cv2.COLOR_BGR2GRAY) 
    
        contours, hierarchy = cv2.findContours(gray, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
        maskKube_copy = maskKube.copy()
    
        contours, hierarchy = cv2.findContours(image=maskKube, mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_NONE)
        largestCont = 0
        largestBoundRectArea = 0
        x = y = w = h = 0
        lx = ly = lw = lh = 0 
        
        if len(contours) == 0:
            self.notfound = True
            return

        #find largest contour
        RATIO_RANGE = 0.3
        for cont in contours:
            x,y,w,h = cv2.boundingRect(cont)
            area = w * h
            if area > largestBoundRectArea and w / h < 1.0 + RATIO_RANGE and w / h > 1.0 - RATIO_RANGE:
                largestBoundRectArea = area
                largestCont = cont
                lx = x
                ly = y
                lw = w
                lh = h 
        frameDimensions = frame.shape
        self.imgX = int(lx + lw / 2)
        self.imgY = int(ly + lh / 2)
        self.setX(int(lx + lw / 2) - int(frameDimensions[1] / 2))
        self.setY(int(ly + lh / 2) - int(frameDimensions[0] / 2))
        self.setHeight(int(lh))
        self.setWidth(int(lw))
        self.setUpright(False) #don't care if cube is upright

    def drawBoundRect(self, frame, color):
        cv2.rectangle(frame, (int(self.imgX - self.w / 2), int(self.imgY - self.h / 2)), (int(self.imgX + self.w / 2), int(self.imgY + self.h / 2)), color, 4, cv2.LINE_AA)

    #green if frame
    def drawCone(self, frame):
        if self.isUpright():
            cv2.rectangle(frame, (int(self.imgX - self.w / 2), int(self.imgY - self.h / 2)), (int(self.imgX + self.w / 2), int(self.imgY + self.h / 2)), [0,255,0], 4, cv2.LINE_AA)
        else:
            cv2.rectangle(frame, (int(self.imgX - self.w / 2), int(self.imgY - self.h / 2)), (int(self.imgX + self.w / 2), int(self.imgY + self.h / 2)), [0,0,255], 4, cv2.LINE_AA)

if __name__ == "__main__":
    # We're a module, never run anything here
    #TEST CODE DO NOT USE
    cone = GamePiece()
    
    cam = cv2.VideoCapture("/dev/video0")
    
    while True:
        ret, frame = cam.read()
        cone.findCone(frame)

        cone.drawCone(frame)
        cv2.imshow("test", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    pass
else:
    # Run things on import here 
    pass
