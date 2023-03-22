#Kone Kube

A somewhat accurate and simple python module for detecting cones and cubes on the 2023
FIRST robotics game field.

```
import konekube
```

## Cone Detection
Returns a "Cone" object that contains data relating to the detected cone's position and
whether it is upright or not. Checks whether a cone is upright by checking the ratio of
the bounding rectangle and also the pixels in the top half of the bounding rectangle and
the number of the pixels in the bottom half of the bounding rectangle and if the number
of pixels in the top half is less than the number of pixels in the bottom half, the cone
is likely upright. It also checks if the bounding rectangle is taller than it is wide
which also implies the cone is upright. Note that some placements of the cone result in
the system being tricked into whether it is upright or not but it is mostly accurate
enough for the purposes of the competition when the robot is in teleoperated mode.

## Cube Detection
After experimentation, it appears the the cube is best highlighted when we only check
for hue (however this makes it also pick up blue and black but for the purposes of the
competition it works fine enough). The cube detection also roughly checks the aspect
ratio of the bounding rectangle to make sure if the object is a cube.

Code example:

```
frame = camera.read() # get an image from the camera
cone = Vision.GamePeice()
lower = [ h1, s1, v1 ]
upper = [ h2, s2, v2 ]
cone.setLowerColor(np.array(lower, dtype=np.uint8))
cone.setUpperColor(np.array(upper, dtype=np.uint8))
cone.findObject(frame)
```

Code example for DriveToGamepeice Decision code:

```
filter = StateEstimator.StateEstimator()
current_action = Decision.DriveToGamepeice(filter, cameras, nt_table, april_executor, 'drivetocone', 30, 255, 255, 300, 300, "cube_picked_color")
```

This code sample creates a DriveToGampeice object that will attempt to drive to a cone.
The arguments in the constructor tell the object to have an HSV range of 30 for H,
255 for S, 255 for V, and read from the file `cube_picked_color` to get the rough color
of a cube that was determined from the color picker. It also has a target width and height
of 300 pixels which will mean that once the size of the object being found in the frame exceeds
300 pixels in either height or width, the robot will stop moving.
