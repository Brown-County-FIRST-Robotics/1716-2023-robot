import argparse
import os
import glob
import json

if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument('--make_config', action='store_true', help="Create a config file")
    parser.add_argument('--make_camera', action='store_true', help="Create a camera file")

    args = parser.parse_args()
    if args.make_camera:
        config = {}
        while True:
            calibration = input('The calibration file to use ("none" if there is there no calibration):')
            if calibration == 'none' or calibration == '':
                config['calibration'] = None
                break
            if os.path.exists(calibration):
                with open(calibration, 'r') as f:
                    config['calibration'] = json.loads(f.read())
                break
            else:
                print('Please give a valid configuration file, or use none')
        while True:
            role=input('Camera\'s role (apriltag|conecube|all):')
            if role=='apriltag':
                config['role'] = 'apriltag'
                break
            elif role=='conecube':
                config['role'] = 'conecube'
                break
            elif role=='all':
                config['role'] = '*'
                break
            else:
                print(f'"{role}" is not a valid option. The valid options are: apriltag conecube all')
        camera_name=input('Camera name:')
        if input(f'This will create a file called "configs/cameras/{camera_name}.json" (Y/n):') != 'n':
            with open(f'configs/cameras/{camera_name}.json', 'w') as f:
                f.write(json.dumps(config))
                print('Config file written')
        else:
            print('Aborted')
