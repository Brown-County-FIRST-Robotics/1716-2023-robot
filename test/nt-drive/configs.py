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

    if args.make_config:
        print('Making config file')
        config = []
        for i in range(int(input('Number of cameras:'))):
            config.append({})
            while True:
                port=input(f'Camera {i+1} port:')
                if not os.path.exists(port):
                    print('Camera path does not exist')
                else:
                    config[-1]['port']=port
                    break
            while True:
                cam_config=input('Camera config file:')
                if os.path.exists(cam_config):
                    config[-1]['config']=cam_config
                    break
                else:
                    print('Camera config does not exist')
            while True:
                with open(config[-1]['config'], 'r') as f:
                    if json.loads(f.read())['role'] in ['*', 'apriltag']:
                        pos=input('Camera position (x,y,r):')
                        if pos=='':
                            pos='[0,0,0]'
                        pos=json.loads(pos)
                        if type(pos)==list and len(pos)==3:
                            config[-1]['pos']=pos
                            break
                    else:
                        print('This camera does not need a position')
                        config[-1]['pos']=None
        config_fname = input('Name of config file:')
        assert input(
            f'This will create a file called "configs/{config_fname}.json", are you sure(y/n):') == 'y', 'Operation canceled'
        with open(f'configs/{config_fname}.json', 'w') as config_file:
            config_file.write(json.dumps(config))
        print('Config file made')
