from imports import functions as func

import serial
import serial.tools.list_ports as list_ports
import json, time

debug = True

# to make sure that the connection is closed properly if any exception occurs (otherwise device would have to be reconnected after error)
def ConnExceptionHandler(func): 
    def call(*args, **kwargs):
        try: return func(*args, **kwargs)
        except KeyboardInterrupt:
            print ("\nClosing...")
        except Exception as e:
            print ("\nERROR: " + str(e))
        finally:
            args[0].close()
            print ("Serial connection closed.")
            raise SystemExit
    return call


def OpenSerialConn(baud, port):
    s = serial.Serial()
    s.baudrate = baud
    s.port = port
    #s.timeout = 0.000000001
    s.open()
    if debug: print ("Serial connection was opened.")
    return s


@ConnExceptionHandler
def HandleConn(s):
    while True:
        #time.sleep(1./12000)
        #in_str = func.RecString(s)
        #in_str = s.readline()
        i= 0
        i = s.in_waiting
        in_str = ""
        if i > 0:
            #print "1"
            for j in range(i):
                #print j
                in_str += chr(ord(s.read()))
        #print ("1")
        if (debug and in_str):
            print("Received string = " + in_str) # received string is ready, do something about it
        if in_str:
            #print "len(in_str) = " + str(len(in_str))
            if in_str.startswith("{") and in_str.strip().endswith("}"):
                data = json.loads(in_str)
                print (data["UID"])
                print (data["data"])
            
                

if __name__ == "__main__":
    print ("Press ctrl+c when finished to close serial connection properly.")
    nodeMCU_port = func.GetComPortByDesctiptorPhrase(list_ports.comports(), "CP210", debug)
    s = OpenSerialConn(115200, nodeMCU_port)
    HandleConn(s)            


        
