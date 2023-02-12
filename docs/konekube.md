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
