import sys
import os
import random

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import NetworkTables1716

from networktables import NetworkTables

NetworkTables.initialize(server='10.17.16.2')

import unittest


class TestNetworkTables(unittest.TestCase):
    def setUp(self):
        self.TestWrapper = NetworkTables1716.NetworkTablesWrapper()
        self.drive_table = NetworkTables.getTable('1716Drive')
        self.pigeon_table = NetworkTables.getTable('1716Pigeon')

    def tearDown(self):
        pass

    def testDrive(self):
        # Generate drive values
        randX = random.random()
        randY = random.random()
        randR = random.random()

        # Run the drive function
        self.TestWrapper.Drive(randX, randY, randR)

        # Check that all the values were set correctly
        assert self.drive_table.getNumber('X', -1) == randX, "X value not set"
        assert self.drive_table.getNumber('Y', -1) == randY, "Y value not set"
        assert self.drive_table.getNumber('Rotation', -1) == randR, "R value not set"

    def testSwitchToTank(self):
        # Run the test function
        self.TestWrapper.SwitchToTank()

        # Check that all the values were set correctly
        assert self.drive_table.getBoolean('isTank', False), "isTank value not set"
        assert not self.drive_table.getBoolean('isMecanum', True), "isMecanum value not set"

    def testSwitchToMecanum(self):
        # Run the test function
        self.TestWrapper.SwitchToMecanum()

        # Check that all the values were set correctly
        assert not self.drive_table.getBoolean('isTank', True), "isTank value not set"
        assert self.drive_table.getBoolean('isMecanum', False), "isMecanum value not set"

    def testStartToAutoBalance(self):
        # Run the test function
        self.TestWrapper.StartAutoBalance()

        # Check that all the values were set correctly
        assert self.drive_table.getBoolean('startAutoBalance', False), "startAutoBalance value not set"

    def testPickupPortal(self):  # TODO:rename
        self.TestWrapper.PickupPortal()
        assert self.drive_table.getBoolean('pickupPortalPiece', False)

    def testPickupFloor(self):  # TODO:rename
        self.TestWrapper.PickupFloor()
        assert self.drive_table.getBoolean('pickupFloorPiece', False)

    def testDropLow(self):  # TODO:rename
        self.TestWrapper.DropLow()
        assert self.drive_table.getBoolean('dropLow', False)

    def testDropMedium(self):  # TODO:rename
        self.TestWrapper.DropMedium()
        assert self.drive_table.getBoolean('dropMedium', False)

    def testDropHigh(self):  # TODO:rename
        self.TestWrapper.DropHigh()
        assert self.drive_table.getBoolean('dropHigh', False)

    def testDropHighNode(self):  # TODO:rename
        self.TestWrapper.DropHighNode()
        assert self.drive_table.getBoolean('dropHighNode', False)

    def testDropLowNode(self):  # TODO:rename
        self.TestWrapper.DropLowNode()
        assert self.drive_table.getBoolean('dropLowNode', False)

    def testGetGyro(self):
        assert self.TestWrapper.GetGyro() is None, 'GetGyro does not return None when `gyro` has no value'

        randGyro = random.random()
        self.pigeon_table.putNumber('gyro', randGyro)

        assert self.TestWrapper.GetGyro() == randGyro, 'GetGyro does not return correct value'

    def testGetAccel(self):
        assert self.TestWrapper.GetAccel() is None, 'GetAccel does not return None when networktables has no value'

        randXAccel = random.random()
        randYAccel = random.random()
        self.pigeon_table.putNumber('xAccel', randXAccel)
        self.pigeon_table.putNumber('yAccel', randYAccel)

        assert self.TestWrapper.GetAccel()[0] == randXAccel, 'GetAccel does not return correct value'
        assert self.TestWrapper.GetAccel()[1] == randYAccel, 'GetAccel does not return correct value'
