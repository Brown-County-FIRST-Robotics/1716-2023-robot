#!/usr/bin/env python

import logging
from networktables import NetworkTables

IP = "10.17.16.2"

class NetworkTablesWrapper:
    def __init__(self):
        self.drive_table = NetworkTables.getTable('1716Drive')
        self.pigeon_table = NetworkTables.getTable('1716Pigeon')
        self.game_table=NetworkTables.getTable('1716GameInfo')
        self.encoder_table=NetworkTables.getTable('1716Encoder')

    def Drive(self, x, y, r):
        self.drive_table.putNumber("X", x)
        self.drive_table.putNumber("Y", y)
        self.drive_table.putNumber("Rotation", r)

    def SwitchToTank(self):
        self.drive_table.putBoolean('isTank', True)
        self.drive_table.putBoolean('isMecanum', False)

    def SwitchToMecanum(self):
        self.drive_table.putBoolean('isTank', False)
        self.drive_table.putBoolean('isMecanum', True)

    def StartAutoBalance(self):
        self.drive_table.putBoolean('startAutoBalance', True)

    def PickupPortal(self):  # TODO:rename
        self.drive_table.putBoolean('pickupPortalPiece', True)

    def PickupFloor(self):  # TODO:rename
        self.drive_table.putBoolean('pickupFloorPiece', True)

    def DropLow(self):  # TODO:rename
        self.drive_table.putBoolean('dropLow', True)

    def DropMedium(self):  # TODO:rename
        self.drive_table.putBoolean('dropMedium', True)

    def DropHigh(self):  # TODO:rename
        self.drive_table.putBoolean('dropHigh', True)

    def DropHighNode(self):  # TODO:rename
        self.drive_table.putBoolean('dropHighNode', True)

    def DropLowNode(self):  # TODO:rename
        self.drive_table.putBoolean('dropLowNode', True)


    def GetAccel(self):
        xAccel = self.pigeon_table.getNumber('xAccel', -1)
        yAccel = self.pigeon_table.getNumber('yAccel', -1)
        if xAccel==-1 or yAccel==-1:
            print(f'No {"y" if yAccel==-1 else "x"}Accel value has been set. ')
            return None
        return xAccel, yAccel

    def GetGyro(self):
        gyro = self.pigeon_table.getNumber('gyro', -1)
        if gyro==-1:
            print('No gyro value')
            return None
        return gyro

    def GetIsAutonomous(self): # TODO:add tests
        val = self.game_table.getBoolean('isAutonomous',False)
        return val

    def GetIsTeleop(self): # TODO:add tests
        val = self.game_table.getBoolean('isTeleop', False)
        return val

    def GetIsRedAlliance(self): # TODO:add tests
        val = self.game_table.getBoolean('isRedAlliance', False)
        return val

    def GetMatchTime(self): # TODO:add tests
        match_time = self.pigeon_table.getNumber('matchTime', -1)
        if match_time==-1:
            print('No match_time value')
            return None
        return match_time

    def GetEncoderVals(self):
        encvals=[
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
        self.encoder_table.putBoolean('encoderReset',True)



if __name__ == "__main__":
    # TEST CODE

    # We're a module, never run anything here
    NetworkTables.initialize(server=IP)
else:
    # Run things on import here
    # initialize Network tables
    NetworkTables.initialize(server=IP)
    pass
