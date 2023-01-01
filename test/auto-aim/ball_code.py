import cv2, time
from networktables import NetworkTables
import random

# Create a VideoCapture object
capture = cv2.VideoCapture(0)
NetworkTables.initialize(server="10.17.16.2")

table = NetworkTables.getTable("balls")
table.putNumber("up_down", 0)
table.putNumber("left_right", 0)
table.putNumber("radius", 0)



# Check if the camera is opened correctly
if not capture.isOpened():
    raise Exception("Could not open camera")

# Set the camera resolution
capture.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

# Read the first frame
while True:
	success, image = capture.read()

	if not success:
	    print("Could not read image from camera") 
	    continue

	# Convert the image to grayscale
	gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

	# Apply a Gaussian blur to the image
	blurred_image = cv2.GaussianBlur(gray_image, (9, 9), 0)

	# Use the Hough transform to detect circles in the image
	circles = cv2.HoughCircles(blurred_image, cv2.HOUGH_GRADIENT, 1, 20, param1=50, param2=60, minRadius=0, maxRadius=0)
	if circles is None:
	    #print("no circles")
	    continue
	# Convert the circles list to a format that can be used with Open  CV
	circles = circles[0]
	truecircles=[]
	for circle in circles:
		x, y, r = [int(i) for i in circle]
		if 256-image[y][x][0]+60-image[y][x][1]+image[y][x][2]<190:
			truecircles.append((x,y,r))
	if len(truecircles)==1:
		table.putNumber("up_down", truecircles[0][1])
		table.putNumber("left_right", truecircles[0][0])
		table.putNumber("radius", truecircles[0][2])
		table.putNumber("random", random.randint(1,1000))
	# Display the image
	#time.sleep(1)
	del image



