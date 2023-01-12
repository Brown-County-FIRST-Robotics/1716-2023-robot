# DEATH_STARE

DEATH_STARE (Driver Extension App To Hopefull See The Action Really Easily) is an app that we built
to more easily tell what is going on in the field from the Robot's POV.

## Why?

During the 2022 practice competition, we were trying to drive the robot but we did not have any
form of vision of the field and we ended up just crashing the robot everywhere and it was very
unintuitive to steer and move around the field and we did not do very well. We saw that other
teams at least had some form of camera that gave a first person point of view of what their
robot "saw" and this was the inspiration for DEATH_STARE. DEATH_STARE was initially created
with the intent that we would be able to flip through different cameras on the robot to get a
first person perspective from that camera. However, since then we have added some more features
to DEATH_STARE.

## Multiple Cameras

DEATH_STARE uses OpenCV to open and display cameras and Flask as a web server to deliver the
output to a browser. However, Cameras consume USB bandwidth and we were using a USB dongle
to store more cameras in a single USB port and maximize the number of cameras that we can have.
However, this resulted in only one camera being opened due to lack of available USB bandwidth.
Our initial solution was simple and we just reduced the resolution of the camera which allowed
up to two cameras per dongle.

## Threading

We then attempted to display multiple cameras at once but it was fairly slow to open them all as
OpenCV has to do a lot of expensive operations to open a camera on the system and that means that
we needed to add some threading. We created a function that would open a camera and we then
used the standard python threading library and spawned a bunch of threads that would open the
cameras in parallel.

However, we later took advantage of Flask's multithreading abilities and instead used that instead
for the view of the side cameras (inspired by a parking app on a mentor's car which helped him
park his car more easily). This allows us to load pages more quickly. 
