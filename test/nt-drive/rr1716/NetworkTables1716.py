#!/usr/bin/env python

import logging
from networktables import NetworkTables

IP = "10.17.16.2"

class NetworkTablesWrapper:
    def __init__(self):
        self.drive_table = NetworkTables.getTable('1716Drive')
        self.pigeon_table = NetworkTables.getTable('1716Pigeon')

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

    def StartPickup(self):  # TODO:rename
        self.drive_table.putBoolean('pickupPiece', True)

    def StartDrop(self):  # TODO:rename
        self.drive_table.putBoolean('startAutoBalance', True)


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

if __name__ == "__main__":
    # TEST CODE

    # We're a module, never run anything here
    NetworkTables.initialize(server=IP)
else:
    # Run things on import here
    # initialize Network tables
    NetworkTables.initialize(server=IP)
    pass
