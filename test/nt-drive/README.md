# This the netowrk tables driving readme!

# <strong>DO NOT EVER CHANGE A CONFIG FILE</strong>
Just make your own

## Config Files
The config files are now seperated into camera files and config files. The camera file stores the camera's calibration and role, while the config file contains the port, camera file, and position on the robot. 

### Making config files
```shell
# to make a camera config
./configs.py --make_camera
# to make a config
./configs.py --make_config
```

These are the modules

## DEATHSTARE
Give the input of the cameras to the driver

## April Tags
Detects the visible april tags on the field

## Vision
Sends what the robot is seeing to the network table(s)

## Network Tablesz
Tells the robot what to do

# Testing
This project uses pytest for testing objects

## How to run the tests
run `pip install pytest` (This installs pytest)

run `pytest`

## These are the files for the cameras
|camera setting             | value |
| ----------- | ----------- |
|brightness	                |  0|
|contrast	                |32|
|saturaton	                |128|
|hue	                    |0|
|white balance auto	        |0|
|gamma	                    |100|
|gain	                    |0|
|power line frequency	    |2|
|white balance temp	        |3200|
|sharpness	                |2|
|backlight compensation	    |1|
|auto exposure	            |3|
|exposure time absolute	    |157|
|exposure dynamic framerate	|1|
