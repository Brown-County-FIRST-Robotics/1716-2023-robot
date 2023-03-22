from networktables import NetworkTables
import threading
import apriltag
import math

cond = threading.Condition()
notified = [False]

def connectionListener(connected, info):
    print(info, '; Connected=%s' % connected)
    with cond:
        notified[i] = True
        cond.notify()

def main():
    
    #Initialize networktables
    NetworkTables.initialize(server="10.17.16.2")
    #NetworkTables.addConnectionListener(connectionListener, immediateNotify=True) 

    #Wait until you connect to network tables
    #with cond:
    #    print("Waiting")
    #    if not notified[0]:
    #        cond.wait()
    print("Connected!")

    #This will be the network table that will store the position of an april tag
    #will store the largest april tag
    table = NetworkTables.getTable("apriltag")

    while True:
        #only for debug
        print("April tag data: "); 
        print(str(table.getNumber("x", 0))) 
        print(str(table.getNumber("y", 0)))
        print(str(table.getNumber("side", 0)))
        print("-------------------")

if __name__ == "__main__":
    main()
