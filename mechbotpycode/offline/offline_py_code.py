import random
import time
import RPi.GPIO as GPIO
import serial
import Encoder


enc = Encoder.Encoder(18, 17)  # (DT,CLK)
enc2 = Encoder.Encoder(19, 26)

mega_connected = False
flysky_connected = False
motordriver_connected = False
status_check='initial'

while not mega_connected:
    try:
        ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1) #9600 is baud rate(must be same with that of NodeMCU)
        mega_connected = True
    except:
        try:
            ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)  # 9600 is baud rate(must be same with that of NodeMCU)
            mega_connected = True
        except:
            print("check serial connection(Mega)")
print("Serial connected")


GPIO.setwarnings(False)

GPIO.setmode(GPIO.BCM)

throttle = 12  # PWM pin connected to Throttle wire
brake_pin = 11
reverse_pin = 10
N_pin = 9
PWM = [23,20]
DIR = [24,21]

#     Data require global
throttle_speed = 0
brake_speed = 0
drive_mode =0
sterring_position=0
sterring_input=0
brake_pwm = []
throttle_pwm = 0

def setup():
    global brake_pwm,throttle_pwm
    GPIO.setup(throttle, GPIO.OUT)
    GPIO.setup(PWM[0], GPIO.OUT)
    GPIO.setup(PWM[1], GPIO.OUT)
    GPIO.setup(DIR[0], GPIO.OUT)  # set pin as output
    GPIO.setup(DIR[1], GPIO.OUT)  # set pin as output

    GPIO.setup(brake_pin, GPIO.OUT)  # set pin as output
    GPIO.setup(reverse_pin, GPIO.OUT)  # set pin as output
    GPIO.setup(N_pin, GPIO.OUT)  # set pin as output

    brake_pwm = [GPIO.PWM(PWM[0], 1000),GPIO.PWM(PWM[1], 1000)]

    throttle_pwm = GPIO.PWM(throttle, 25000)  # create PWM instance with frequency


def get_data():
    global throttle_speed,brake_speed,drive_mode,sterring_input,sterring_position,mega_connected,flysky_connected,motordriver_connected
    try:
        if ser.in_waiting > 0:
            mega_connected = True
            response = ser.readline().decode('utf-8').rstrip()
            print(response)
            data = response.split("#")

            if(len(data)==8):
                throttle_speed = int(data[1])
                brake_speed = int(data[2])
                drive_mode = int(data[3])
                if(drive_mode >= 1900):
                    drive_mode = 'N'
                elif(drive_mode >= 1400 and drive_mode <= 1600):
                    drive_mode = 'F'
                else:
                    drive_mode = 'R'
                sterring_input = int(data[4])
                sterring_position = int(data[5])

                flysky_connected = True
                mega_connected = True
                motordriver_dconnected = True




    except:
        mega_connected = False



###################
def drive(drive_speed, drive_mode):
    if (drive_speed >= 100):
        drive_speed = (abs(drive_speed)/abs(drive_speed))*99

    if(drive_mode == 'N'):
        GPIO.output(N_pin, GPIO.HIGH)
        GPIO.output(reverse_pin, GPIO.LOW)
        throttle_pwm.start(0)
    elif(drive_mode == 'R'):
        GPIO.output(reverse_pin, GPIO.HIGH)
        GPIO.output(N_pin, GPIO.LOW)
        throttle_pwm.start(drive_speed)
    elif(drive_mode == 'F'):
        GPIO.output(N_pin, GPIO.LOW)
        GPIO.output(reverse_pin,GPIO.LOW)
        throttle_pwm.start(drive_speed)
    else:
        GPIO.output(N_pin, GPIO.HIGH)
        GPIO.output(reverse_pin,GPIO.HIGH)
        throttle_pwm.start(0)




    print(throttle_speed)


def brakeing(input,encoder1,encoder2 ):

    error_encoder1 = input - encoder1
    error_encoder2 = input - encoder2

    if(brake_speed> 20):
        GPIO.output(brake_pin, GPIO.HIGH)
    else:
        GPIO.output(brake_pin, GPIO.LOW)


    if(error_encoder1 > 3):
        brake_pwm[0].start(100)
        GPIO.output(DIR[0], GPIO.HIGH)
    elif (error_encoder1 < -3):
        brake_pwm[0].start(100)
        GPIO.output(DIR[0], GPIO.LOW)
    else:
        brake_pwm[0].start(0)
        GPIO.output(DIR[0], GPIO.LOW)

    if(error_encoder2 > 3):
        brake_pwm[1].start(100)
        GPIO.output(DIR[1], GPIO.HIGH)
    elif (error_encoder1 < -3):
        brake_pwm[1].start(100)
        GPIO.output(DIR[1], GPIO.LOW)
    else:
        brake_pwm[1].start(0)
        GPIO.output(DIR[1], GPIO.LOW)

def stop():
    brake_pwm[0].start(0)
    brake_pwm[0].start(0)
    throttle_pwm.start(0)

def check_error():
    global mega_connected, status_check
    if not (mega_connected):
        status_check= "mega_disconneced"
        #print("mega_disconneced")
        stop()
    elif not (flysky_connected):
        status_check = "flysky_disconneced"
        stop()
        #print("mega_disconneced")
    elif not (motordriver_connected):
        status_check = "motor_driver_disconnected"
        stop()
        #print("motor_driver_disconnected")
    else:
        status_check = "OK"


setup()
while True:
    start_time = time.time()
    drive(throttle_speed, drive_mode)
    encoder1 = enc.read()
    encoder2 = enc2.read()
    brakeing(brake_speed, encoder1, encoder2)
    check_error()
    #print(encoder1,encoder2)
    print("throttle"+str(throttle_speed)+" brake "+str(brake_speed)+" sterring in "+str(sterring_input)+" sterring pos "
          +str(drive_mode)+" loop time pi "++" status "+str(status_check))
    loop_time = time.time() - start_time