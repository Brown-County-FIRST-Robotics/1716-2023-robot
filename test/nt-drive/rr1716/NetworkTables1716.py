#!/usr/bin/env python

import logging
from networktables import NetworkTables

IP = "10.17.16.2"


class NetworkTablesWrapper:
    def __init__(self):
        self.drive_table = NetworkTables.getTable('1716Drive')
        self.pigeon_table = NetworkTables.getTable('1716Pigeon')
        self.game_table = NetworkTables.getTable('1716GameInfo')
        self.encoder_table = NetworkTables.getTable('1716Encoder')


    def Drive(self, x, y, r):
        self.drive_table.putNumber("x", x)
        self.drive_table.putNumber("y", y)
        self.drive_table.putNumber("rotation", r)


    def SwitchToTank(self):
        self.drive_table.putBoolean('isTank', True)


    def SwitchToMecanum(self):
        self.drive_table.putBoolean('isTank', False)


    def StartAutoBalance(self):
        self.drive_table.putBoolean('startAutoBalance', True)


    def EndAutoBalance(self):
        self.drive_table.putBoolean('startAutoBalance', True)


    def SetArmPortal(self):  # TODO:rename
        self.drive_table.putBoolean('setArmPortal', True)


    def SetArmFloor(self):  # TODO:rename
        self.drive_table.putBoolean('setArmFloor', True)


    def PickupObject(self):
        self.drive_table.putBoolean('pickupObject', True)


    def SetArmLow(self):  # TODO:rename
        self.drive_table.putBoolean('setArmLow', True)


    def SetArmMedium(self):  # TODO:rename
        self.drive_table.putBoolean('setArmMedium', True)


    def SetArmHigh(self):  # TODO:rename
        self.drive_table.putBoolean('setArmHigh', True)


    def SetArmHighNode(self):  # TODO:rename
        self.drive_table.putBoolean('setArmHighNode', True)


    def SetArmLowNode(self):  # TODO:rename
        self.drive_table.putBoolean('setArmLowNode', True)


    def IsArmDone(self):
        arm_values=[
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
        self.drive_table.putBoolean('dropObject', True)


    def GetAccel(self):
        x_accel = self.pigeon_table.getNumber('xAccel', -1)
        y_accel = self.pigeon_table.getNumber('yAccel', -1)
        if x_accel == -1 or y_accel == -1:
            print(f'No {"y" if y_accel == -1 else "x"}Accel value has been set. ')
            return None
        return x_accel, y_accel


    def GetGyro(self):
        gyro = self.pigeon_table.getNumber('gyro', -1)
        if gyro == -1:
            print('No gyro value')
            return None
        return gyro


    def GetIsAutonomous(self):  # TODO:add tests
        val = self.game_table.getBoolean('isAutonomous', False)
        return val


    def GetIsTeleop(self):  # TODO:add tests
        val = self.game_table.getBoolean('isTeleop', False)
        return val


    def GetIsRedAlliance(self):  # TODO:add tests
        val = self.game_table.getBoolean('isRedAlliance', False)
        return val


    def GetMatchTime(self):  # TODO:add tests
        match_time = self.pigeon_table.getNumber('matchTime', -1)
        if match_time == -1:
            print('No match_time value')
            return None
        return match_time


    def GetEncoderVals(self):
        encvals = [
            self.encoder_table.getNumber('frontLeftEncoder', '-1'),
            self.encoder_table.getNumber('backLeftEncoder', '-1'),
            self.encoder_table.getNumber('frontRightEncoder', '-1'),
            self.encoder_table.getNumber('backRightEncoder', '-1')
        ]
        if -1 in encvals:
            print('Encoder value missing')
            return None
        return encvals


    def ResetEncoderVals(self):
        self.encoder_table.putBoolean('encoderReset', True)


if __name__ == "__main__":
    # TEST CODE

    # We're a module, never run anything here
    NetworkTables.initialize(server=IP)
else:
    # Run things on import here
    # initialize Network tables
    NetworkTables.initialize(server=IP)
    pass
