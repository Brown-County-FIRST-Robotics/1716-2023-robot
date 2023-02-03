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
        tables["name"] = NetworkTables.getTable(name)
    return tables

if __name__ == "__main__":
    # We're a module, never run anything here
    NetworkTables.initialize(server=IP)
    getTables([ "1716Drive" ])
    pass
else:
    # Run things on import here
    #initialize Network tables
    NetworkTables.initialize(server=IP)
    pass
