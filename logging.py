import csv
import time
from datetime import datetime
import json
import serial

startTime = datetime.now()
newFilePeriod = 3600
readPeriod = 0.5
arduino = serial.Serial(port="/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_95032303637351609161-if00", 
                        baudrate=115200, 
                        timeout=.1)

def read():
    data = arudino.read_until('}')
    return data

def WriteToCSV(delta):
    # Gets the current filename of the CSV file formatted
    # yyyy_mm_dd-hh_mm_ss
    title = startTime.strftime("%Y_%m_%d-%H_%M_%S.csv")

    # TODO: Gather data from the Serial port instead of just static numbers.
    rawJSON = read()
    jsonDict = json.loads(rawJSON)
    data = [datetime.now().strftime("%H:%M:%S"), jsonDict["V1"], jsonDict["V2"], jsonDict["WSpd"]]

    # Opens a new file in append mode, this will create a new one
    # if one doesnt already exist and if it does just add the rows onto the end.
    with open(title, mode='a', encoding='UTF8', newline='') as wrt:
        writer = csv.writer(wrt)
        writer.writerow(data)

while True:
    currentTime = datetime.now()
    delta = abs((startTime - currentTime).total_seconds())

    # Checks if the current file has been open for over 1hr
    # Changes the starTime variable to the new time to allow
    # for another file to be created. 
    if delta >= newFilePeriod:
        startTime = datetime.now()

    # Writes current recieved data to the CSV file.
    WriteToCSV(delta)

    # TODO Using sleep seems a bit shit but will do for now.
    time.sleep(readPeriod)
