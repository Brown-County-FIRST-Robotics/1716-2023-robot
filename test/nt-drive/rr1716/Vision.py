#!/usr/bin/env python

import logging

#Object - can represent a cone or a cube
class Object():
    # position of cone in the image in terms of pixels
    x = 0
    y = 0
    #dimensions of bounding rectangle
    w = 0
    h = 0
    # is the cone upright?
    upright = False #Do not care about this value if this is a cube

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

# Returns a cone object when it attempts to find a
# cone in an image (frame)
# also passes in the lower color of the cone and upper color of the cone
def findCone(frame, lower_color_kone, upper_color_kone):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    maskKone = cv2.inRange(hsv, lower_color_kone, upper_color_kone)
         
    gray = cv2.cvtColor(hsv, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
       
    # cut out anything that isn't the cone
    for i in range(len(blur)):
        for j in range(len(blur[i])): 
            if maskKone[i][j]:
                continue
            blur[i,j] = 0 

    contours, hierarchy = cv2.findContours(gray, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
    maskKone_copy = maskKone.copy()

    contours, hierarchy = cv2.findContours(image=maskKone, mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_NONE)
    largestCont = 0
    largestBoundRectArea = 0
    x = y = w = h = 0
    lx = ly = lw = lh = 0 
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

    kone = Object()
    kone.setX(int(lx + lw / 2))
    kone.setY(int(ly + lh / 2))
    kone.setHeight(int(lh))
    kone.setWidth(int(lw))
    kone.setUpright(upright)
    return kone

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

# Returns a cube object when it attempts to find a
# cube in an image (frame)
# also passes in the lower color of the cube and upper color of the cube 
# NOTE: this also does a ratio check to roughly guess if it is a cube
def findCube(frame, lower_color_kone, upper_color_kone):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    maskKone = cv2.inRange(hsv, lower_color_kone, upper_color_kone)
         
    gray = cv2.cvtColor(hsv, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
       
    # cut out anything that isn't the cone
    for i in range(len(blur)):
        for j in range(len(blur[i])): 
            if maskKone[i][j]:
                continue
            blur[i,j] = 0 

    contours, hierarchy = cv2.findContours(gray, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) 
    maskKone_copy = maskKone.copy()

    #debug, remove later
    cv2.imshow("mask", maskKone_copy) 

    contours, hierarchy = cv2.findContours(image=maskKone, mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_NONE)
    largestCont = 0
    largestBoundRectArea = 0
    x = y = w = h = 0
    lx = ly = lw = lh = 0 
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

    kone = Object()
    kone.setX(int(lx + lw / 2))
    kone.setY(int(ly + lh / 2))
    kone.setHeight(int(lh))
    kone.setWidth(int(lw))
    kone.setUpright(False) #don't care if cube is upright
    return kone

if __name__ == "__main__":
    # We're a module, never run anything here
    pass
else:
    # Run things on import here
    
    pass
