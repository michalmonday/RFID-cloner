from imports import functions as func

from serial import Serial
import serial.tools.list_ports as list_ports

debug = True

# to make sure that the connection is closed properly if any exception occurs (otherwise device would have to be reconnected after error)
def ConnExceptionHandler(func): 
    def call(*args, **kwargs):
        try: return func(*args, **kwargs)
        except KeyboardInterrupt:
            print "\nClosing..."
        except Exception as e:
            print "\nERROR: " + str(e)
        finally:
            args[0].close()
            print "Serial connection closed."
            raise SystemExit
    return call


def OpenSerialConn(baud, port):
    s = Serial()
    s.baudrate = baud
    s.port = port
    s.open()
    if debug: print "Serial connection was opened."
    return s


@ConnExceptionHandler
def HandleConn(s):
    while True:
        in_str = func.RecString(s, 0.001)
        if debug and in_str: print "Received string = " + in_str # received string is ready, do something about it
        if in_str:
            print "len(in_str) = " + str(len(in_str))
            #if in_str.startswith("RFID_data:") and "UID_data:" in in_str:
            
                

if __name__ == "__main__":
    print "Press ctrl+c when finished to close serial connection properly."
    nodeMCU_port = func.GetComPortByDesctiptorPhrase(list_ports.comports(), "CP210", debug)
    s = OpenSerialConn(115200, nodeMCU_port)
    HandleConn(s)            


        
