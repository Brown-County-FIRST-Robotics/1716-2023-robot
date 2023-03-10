#!/usr/bin/env python

import logging
from networktables import NetworkTables

IP = '10.17.16.2'


class NetworkTablesWrapper:
    def __init__(self):
        logging.debug('NetworkTablesWrapper.init')
        self.drive_table = NetworkTables.getTable('1716Drive')
        self.pigeon_table = NetworkTables.getTable('1716Pigeon')
        self.game_table = NetworkTables.getTable('1716GameInfo')
        self.encoder_table = NetworkTables.getTable('1716Encoder')
        self.dashboard_table = NetworkTables.getTable("1716DashboardInput")
        self.motor_table = NetworkTables.getTable("1716Motors")


    def Drive(self, x, y, r):
        logging.info(f'NetworkTablesWrapper.Drive({x},{y},{r})')
        self.drive_table.putNumber('x', x)
        self.drive_table.putNumber('y', y)
        self.drive_table.putNumber('rotation', r)

    def SwitchToTank(self):
        logging.debug(f'NetworkTablesWrapper.SwitchToTank()')
        self.drive_table.putBoolean('isTank', True)

    def SwitchToMecanum(self):
        logging.debug(f'NetworkTablesWrapper.SwitchToMecanum()')
        self.drive_table.putBoolean('isTank', False)

    def StartAutoBalance(self):
        logging.debug(f'NetworkTablesWrapper.StartAutoBalance()')
        self.drive_table.putBoolean('startAutoBalance', True)

    def EndAutoBalance(self):
        logging.debug(f'NetworkTablesWrapper.EndAutoBalance()')
        self.drive_table.putBoolean('startAutoBalance', False)

    def SetArmPortal(self):  # TODO:rename
        logging.debug(f'NetworkTablesWrapper.SetArmPortal()')
        self.drive_table.putBoolean('setArmPortal', True)

    def SetArmFloor(self):  # TODO:rename
        logging.debug(f'NetworkTablesWrapper.SetArmFloor()')
        self.drive_table.putBoolean('setArmFloor', True)

    def PickupObject(self):
        logging.debug(f'NetworkTablesWrapper.PickupObject()')
        self.drive_table.putBoolean('pickupObject', True)

    def SetArmLow(self):  # TODO:rename
        logging.debug(f'NetworkTablesWrapper.SetArmLow()')
        self.drive_table.putBoolean('setArmLow', True)

    def SetArmMedium(self):  # TODO:rename
        logging.debug(f'NetworkTablesWrapper.SetArmMedium()')
        self.drive_table.putBoolean('setArmMedium', True)

    def SetArmHigh(self):  # TODO:rename
        logging.debug(f'NetworkTablesWrapper.SetArmHigh()')
        self.drive_table.putBoolean('setArmHigh', True)

    def SetArmHighNode(self):  # TODO:rename
        logging.debug(f'NetworkTablesWrapper.SetArmHighNode()')
        self.drive_table.putBoolean('setArmHighNode', True)

    def SetArmLowNode(self):  # TODO:rename
        logging.debug(f'NetworkTablesWrapper.SetArmLowNode()')
        self.drive_table.putBoolean('setArmLowNode', True)

    def RetractArm(self):
        logging.debug(f'NetworkTablesWrapper.RetractArm()')
        self.drive_table.putBoolean('retractArm', True)

    def IsArmDone(self):
        logging.debug(f'NetworkTablesWrapper.IsArmDone()')
        arm_values = [
            self.drive_table.getBoolean('setArmLowNode', False),
            self.drive_table.getBoolean('setArmHighNode', False),
            self.drive_table.getBoolean('setArmLow', False),
            self.drive_table.getBoolean('setArmMedium', False),
            self.drive_table.getBoolean('setArmHigh', False),
            self.drive_table.getBoolean('setArmPortal', False),
            self.drive_table.getBoolean('setArmFloor', False),
        ]
        return True not in arm_values

    def DropObject(self):
        logging.debug(f'NetworkTablesWrapper.DropObject()')
        self.drive_table.putBoolean('dropObject', True)

    def GetAccel(self):
        logging.debug(f'NetworkTablesWrapper.GetAccel()')
        x_accel = self.pigeon_table.getNumber('xAccel', -1)
        y_accel = self.pigeon_table.getNumber('yAccel', -1)
        z_accel = self.pigeon_table.getNumber('zAccel', -1)
        if x_accel == -1 or y_accel == -1 or z_accel == -1:
            print(f'No {"y" if y_accel == -1 else "x"}Accel value has been set. ')
            return None
        return x_accel, y_accel, z_accel

    def GetYaw(self):
        logging.debug(f'NetworkTablesWrapper.GetYaw()')
        yaw = self.pigeon_table.getNumber('yaw', -1)
        if yaw == -1:
            print('No yaw value')
            return None
        return yaw

    def IsAutonomous(self):  # TODO:add tests
        logging.debug(f'NetworkTablesWrapper.IsAutonomous()')
        val = self.game_table.getBoolean('isAutonomous', False)
        return val

    def IsTeleop(self):  # TODO:add tests
        logging.debug(f'NetworkTablesWrapper.IsTeleop()')
        val = self.game_table.getBoolean('isTeleop', False)
        return val

    def IsRedAlliance(self):  # TODO:add tests
        logging.debug(f'NetworkTablesWrapper.IsRedAlliance()')
        val = self.game_table.getBoolean('isRedAlliance', False)
        return val

    def GetMatchTime(self):  # TODO:add tests
        logging.debug(f'NetworkTablesWrapper.GetMatchTime()')
        match_time = self.pigeon_table.getNumber('matchTime', -1)
        if match_time == -1:
            print('No match_time value')
            return None
        return match_time

    def GetEncoderVals(self):
        logging.debug(f'NetworkTablesWrapper.GetEncoderVals()')
        encoder_values = [
            self.encoder_table.getNumber('frontLeftEncoder', -1),
            self.encoder_table.getNumber('backLeftEncoder', -1),
            self.encoder_table.getNumber('frontRightEncoder', -1),
            self.encoder_table.getNumber('backRightEncoder', -1)
        ]
        if -1 in encoder_values:
            logging.warning('Encoder value missing. returning 0 instead')
            return [0,0,0,0]
        return encoder_values

    def ResetEncoderVals(self):
        logging.debug(f'NetworkTablesWrapper.ResetEncoderVals')
        self.encoder_table.putBoolean('resetEncoder', True)

    def GetPlacement(self):
        logging.debug(f'NetworkTablesWrapper.GetPlacement')
        pos = self.dashboard_table.getNumberArray('placePos',[-1,-1])
        if pos==[-1,-1]:
            return None
        return pos

    def GetPickup(self):
        logging.debug(f'NetworkTablesWrapper.GetPickup')
        pos=self.dashboard_table.getNumber('pickupPos',-1)
        if pos==-1:
            return None
        return pos
    def GetMotors(self, field_r):
        cx = math.cos(field_r * math.pi / 180)
        cy = math.sin(field_r * math.pi / 180)

        ax = math.cos((field_r + 90) * math.pi / 180)
        ay = math.sin((field_r + 90) * math.pi / 180)

        move_x=240*self.motor_table.getNumber('x',0)
        move_y=240*self.motor_table.getNumber('y',0)

        fx = move_x * cx + move_y * cy
        fy = move_x * ax + move_y * ay
        fr = 300*self.motor_table.getNumber('r',0)
        return fx,fy,fr

if __name__ == '__main__':
    # TEST CODE

    # We're a module, never run anything here
    NetworkTables.initialize(server=IP)
else:
    # Run things on import here
    # initialize Network tables
    NetworkTables.initialize(server=IP)
    pass
