#!/usr/bin/python

import uhal
def write_register(name,value):
    hw.getNode(name).write(value)
    hw.dispatch()
    var = hw.getNode(name).read()
    hw.dispatch()
    print "{0:} = {1:}".format(name,value)
    hw.dispatch()


uhal.setLogLevelTo(uhal.LogLevel.WARNING)
manager = uhal.ConnectionManager("file://connection.xml")
hw = manager.getDevice("tdc1")

for i in range(65):
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
#    if (i == 3 or i==5):
        write_register("TDC_{0:02}.WRITE.EN".format(i),0)
#    else:
#        write_register("TDC_{0:02}.WRITE.EN".format(i),0)

#Trigger Control

write_register("TDC_64.WRITE.EN",1)

#write_register("TRIGGER_WINDOW",8)
#write_register("SIGNAL_DELAY",266)

write_register("SIGNAL_DELAY",10)
write_register("SIGNAL_LATENCY",0)
write_register("TRIGGER_WINDOW",5)
write_register("RESET_IPB",1)
write_register("RESET_IPB",0)
write_register("SRC_IP",0x0A10060A)
write_register("DST_IP",0x0A984803)
write_register("PORT_REG.SRC_PORT",50)
write_register("PORT_REG.DST_PORT",45454)

