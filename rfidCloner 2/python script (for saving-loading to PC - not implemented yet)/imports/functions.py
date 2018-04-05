'''
NodeMCU is shown as the following on my PC:
product None
name None
vid 4292
hwid USB VID:PID=10C4:EA60 SER=0001 LOCATION=1-2
pid 60000
interface None
location 1-2
device COM5
serial_number 0001
manufacturer None
description Silicon Labs CP210x USB to UART Bridge (COM5)

using the last line (description) it will be selected from other com ports, using "CP210" phrase
'''

import time

millis = time.time # just 'changing' the name of the function
delay_sec = time.sleep # for consistency with Arduino (_sec because argument is in seconds - use 0.001 for 1ms)
last_byte_rec_time = 0

def GetComPortByDesctiptorPhrase(ports, phrase, debug):
    if debug: print "\nAvailable serial ports with details: "
    for p in ports:
        if debug:
            print ""
            for k,v in vars(p).iteritems():
                print k,v
        if phrase in str(p.description):
            print "NodeMCU com port found. (" + str(p.device) + ")"
            return p.device
    print "\nCouldn't find NodeMCU by the following descriptor phrase: 'CP210'"
    print "Exited program at functions::GetComPortByDesctiptorPhrase()"
    raise SystemExit

def RecString(s, delay):
    delay_sec(delay) # to avoid unnecessary CPU use (without it my fan became loud)
    if s.in_waiting:  # Or: if s.inWaiting():
        in_str=""
        while millis() - last_byte_rec_time < 0.1:
            while s.in_waiting:
                in_str += s.read()
            last_byte_rec_time = millis()
        return in_str
    return ""
    
