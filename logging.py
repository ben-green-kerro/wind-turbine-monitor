# Importing Libraries
import serial
import time
arduino = serial.Serial(baudrate=115200, timeout=.1)
def read():
    data = arduino.readline()
    return data
while True:
    value = read()
    print(value)