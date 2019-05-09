import serial
import time
import gevent

ser = serial.Serial('COM7', 115200, timeout=10) #timeout=200
ser.flushInput()

if __name__ == "__main__":
    while True:
        try:
            ser.write(b"00")
            print(ser.readline())
            time.sleep(2)
        except gevent.Timeout:
            print('Could not complete in timeout!')
            exit(1)
