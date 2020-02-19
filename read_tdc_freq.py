#!/usr/bin/python
import time
import uhal

#from argparse import ArgumentParser

#uhal.setLogLevelTo(uhal.LogLevel.WARNING)
#manager = uhal.ConnectionManager("file://connection.xml")
#hw = manager.getDevice("tdc")

#var = hw.getNode("SCALER_04").read()
#var = hw.getNode("OPEN_0").read()
#hw.dispatch()
#print ("SCALER_04 =", "%8.8x"% var)
#hw.dispatch()

def read_register(name):
    var = hw.getNode(name).read()
    hw.dispatch()
    if var != 0 :
      print "{0:} = {1:}".format(name,var)
      hw.dispatch()

uhal.setLogLevelTo(uhal.LogLevel.WARNING)
manager = uhal.ConnectionManager("file://connection.xml")
hw = manager.getDevice("tdc")

#read_register("FIRMWARE_ID")

for i in range(65):
    read_register("SCALER_{0:02}".format(i))

var = hw.getNode("FIRMWARE_ID").read()
hw.dispatch()
print "FW ID = ", "%8.8d" % var
