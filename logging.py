import serial
import time
import datetime
arduino = serial.Serial(port="/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_95032303637351609161-if00", baudrate=115200, timeout=.1)
def read():
    data = arduino.readline().strip().decode()
    return data

currenttime = datetime.datetime.now()
hour = currenttime.hour
while True:
    while datetime.datetime.now().hour == hour:
        if arduino.in_waiting:
            txt = read()
            args = txt.split(",")
            print(args)
