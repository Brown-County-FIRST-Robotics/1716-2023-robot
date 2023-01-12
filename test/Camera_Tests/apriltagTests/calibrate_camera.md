# Camera Calibration:
First, get a charuco board. To make one, run the command:
```bash
python3 CalibrateCamera.py generate
```
Then, print the file and glue it to a foam board. Take about 30-40 pictures at all different angles and locations. 

Then, run:
```bash
python3 CalibrateCamera.py ~/Pictures/Webcam/*
```
Warning: this will check for charuco boards in every image you have ever taken. If you have a lot of images, this could take a while



