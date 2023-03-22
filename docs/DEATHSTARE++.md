# DEATHSTARE++

DEATHSTARE++ is just DEATHSTARE with module support and better concurrency. The UI mostly works the same, but some features were removed. 

## Usage
Before you run any python scripts, you need to set your python path
```bash
source makePythonPath.sh
```
This will set your python path to the `test/modules/` folder. If you plan to make any modules, add them there. 

Then, you will need to make your config file. You can do that with:
```bash
python3 main.py --config
```

For each of your cameras, it will ask you about the camera's port, calibration file, and position. If you don't care about the position of the camera, you can just hit enter, and it will default to all zero values. 

Finally, you can run DEATHSTARE++ with:
```bash
python3 main.py configs/{your config name} {networktables server}
```

If you are testing locally, your server should be set to `localhost`, if you are running on the robot, set it to `10.17.16.2`

## Adding your own modules

If you want to add your own module, go to the `handleCam()` function, and look for the comment that says "ADD YOUR OWN MODULES HERE", and insert your code there. 

