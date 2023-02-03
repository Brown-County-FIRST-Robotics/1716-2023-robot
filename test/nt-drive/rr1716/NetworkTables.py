#!/usr/bin/env python

import logging
from networktables import NetworkTables

IP = "10.17.16.2"

#tableNames is an array of strings
#ex: ([ "1716Drive", "foo", "bar", "apriltags" ]
#returns a dictionary of the tables
def getTables(tableNames):
    tables = {}
    for name in tableNames:
        tables[name] = NetworkTables.getTable(name)
    return tables

if __name__ == "__main__":
    #TEST CODE
    import time
    # We're a module, never run anything here
    NetworkTables.initialize(server=IP) 
    timePassed = 0
    while True:
        begin = time.time_ns() 
        tables = getTables([ "1716Drive" ])
        tables["1716Drive"].putNumber("Rotation", 0.1)
        print(tables["1716Drive"].getNumber("Rotation", 0.0))
        print(str(timePassed))
        end = time.time_ns() 
        timePassed = end - begin 
    pass
else:
    # Run things on import here
    #initialize Network tables
    NetworkTables.initialize(server=IP)
    pass
