import random
import time
import RPi.GPIO as GPIO
import serial
import Encoder
    

                                                                                                                                                                                                                                                                                                                                                                                                                                     
mega_connected = False                                                                                                                                                                                                                                                                                                                     
flysky_connected = False
motordriver_connected = False
status_check = 'initial'

while not mega_connected:
    try:
        ser = serial.Serial('/dev/ttyACM0', 115200,
                                timeout=0.025)  
        ser.flush()
        mega_connected = True
    except:
        try:
            ser = serial.Serial('/dev/ttyACM0', 250000,
                                timeout=0.025) 
            ser.flush()
            mega_connected = True
        except:
            print("check serial connection(Mega)")
print("Serial connected")




def get_data():
    global throttle_speed, brake_speed, drive_mode, sterring_input, sterring_position, mega_connected, flysky_connected, motordriver_connected
    try:
        if ser.in_waiting > 0:
            mega_connected = True
            response = ser.readline().decode('utf-8').rstrip()
            print(response)
            data = response.split("#")
            #print(len(data))
            if (len(data) == 10):
                if(int(data[1]) >0):
                    throttle_speed = int((int(data[1])-1000)*0.1)
                #print(throttle_speed)
                if(int(data[2]) >= 1500):
                    brake_speed = (int(data[2])-1500)*6
                drive_mode = int(data[3])
                if (drive_mode >= 1900):
                    drive_mode = 'R'
                elif (drive_mode >= 1400 and drive_mode <= 1600):
                    drive_mode = 'F'
                else:
                    drive_mode = 'N'
                sterring_input = int(data[4])
                sterring_position = int(data[5])

                flysky_connected = True
                mega_connected = True
                motordriver_connected = True




    except:
        mega_connected = False
        
while True:
    get_data()
