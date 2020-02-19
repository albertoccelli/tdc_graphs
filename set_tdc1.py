#!/usr/bin/python

import uhal
def write_register(name,value):
    hw.getNode(name).write(value)
    hw.dispatch()
    var = hw.getNode(name).read()
    hw.dispatch()
    print "{0:} = {1:}".format(name,value)
    hw.dispatch()

delay = 13 #input('Select a delay: ')

uhal.setLogLevelTo(uhal.LogLevel.WARNING)
manager = uhal.ConnectionManager("file://connection.xml")
hw = manager.getDevice("tdc")

write_register("RESET_IPB",1)
write_register("RESET_IPB",0)

for i in range(65):
    if i==32:
        write_register("TDC_{0:02}.WRITE.CNTVALUEIN_0".format(i),0)
        write_register("TDC_{0:02}.WRITE.CNTVALUEIN_1".format(i),delay)
        write_register("TDC_{0:02}.WRITE.LD_0".format(i),0)
        write_register("TDC_{0:02}.WRITE.LD_1".format(i),0)
        write_register("TDC_{0:02}.WRITE.MODE".format(i),0)
        write_register("TDC_{0:02}.WRITE.LD_0".format(i),1)
        write_register("TDC_{0:02}.WRITE.LD_1".format(i),1)
        write_register("TDC_{0:02}.WRITE.LD_0".format(i),0)
        write_register("TDC_{0:02}.WRITE.LD_1".format(i),0)
    elif i==00:
        write_register("TDC_{0:02}.WRITE.CNTVALUEIN_0".format(i),0)
        write_register("TDC_{0:02}.WRITE.CNTVALUEIN_1".format(i),delay)
        write_register("TDC_{0:02}.WRITE.LD_0".format(i),0)
        write_register("TDC_{0:02}.WRITE.LD_1".format(i),0)
        write_register("TDC_{0:02}.WRITE.MODE".format(i),0)
        write_register("TDC_{0:02}.WRITE.LD_0".format(i),1)
        write_register("TDC_{0:02}.WRITE.LD_1".format(i),1)
        write_register("TDC_{0:02}.WRITE.LD_0".format(i),0)
        write_register("TDC_{0:02}.WRITE.LD_1".format(i),0)
    else:
        write_register("TDC_{0:02}.WRITE.CNTVALUEIN_0".format(i),0)
        write_register("TDC_{0:02}.WRITE.CNTVALUEIN_1".format(i),13)
        write_register("TDC_{0:02}.WRITE.LD_0".format(i),0)
        write_register("TDC_{0:02}.WRITE.LD_1".format(i),0)
        write_register("TDC_{0:02}.WRITE.MODE".format(i),0)
        write_register("TDC_{0:02}.WRITE.LD_0".format(i),1)
        write_register("TDC_{0:02}.WRITE.LD_1".format(i),1)
        write_register("TDC_{0:02}.WRITE.LD_0".format(i),0)
        write_register("TDC_{0:02}.WRITE.LD_1".format(i),0)


print "\n\n# enable channels\n"
for i in range(65):
#    if (i==5 or i==10 or i == 15 or i==26 or i==34 or i==38 or i==43 or i==45 or i==46 or i==49 or i==50):
        write_register("TDC_{0:02}.WRITE.EN".format(i),0)
#    else:
#        write_register("TDC_{0:02}.WRITE.EN".format(i),0)

#Trigger Control
write_register("TDC_00.WRITE.EN",1)
write_register("TDC_32.WRITE.EN",1)
write_register("TDC_64.WRITE.EN",1)

#write_register("TRIGGER_WINDOW",8)
#write_register("SIGNAL_DELAY",266)

write_register("SIGNAL_DELAY",300)
write_register("SIGNAL_LATENCY",0)
write_register("TRIGGER_WINDOW",290)
write_register("RESET_IPB",1)
write_register("RESET_IPB",0)
write_register("SRC_IP",0x0A100628)
write_register("DST_IP",0x0A100667)
write_register("PORT_REG.SRC_PORT",50)
write_register("PORT_REG.DST_PORT",60001)
