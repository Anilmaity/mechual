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
                                timeout=0.1)  
        ser.flush()
        mega_connected = True
    except:
        try:
            ser = serial.Serial('/dev/ttyACM1', 115200,
                                timeout=0.1) 
            ser.flush()
            mega_connected = True
        except:
            print("check serial connection(Mega)")
print("Serial connected")

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BCM)


throttle = 13  # PWM pin connected to Throttle wire
brake_pin = 25
N_pin = 8
reverse_pin = 7
PWM = [20, 12]
DIR = [21, 16]
ENC1 = [26,19]
ENC2 = [18,23]
throttle_pwm =0
brake_pwm = []

def setup():
    global throttle_pwm,brake_pwm
    GPIO.setup(ENC1[0], GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(ENC1[1], GPIO.IN, pull_up_down=GPIO.PUD_UP)
        
    GPIO.add_event_detect(ENC1[0], GPIO.RISING, callback=rotation_decode1)
        
    GPIO.setup(ENC2[0], GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(ENC2[1], GPIO.IN, pull_up_down=GPIO.PUD_UP)

    GPIO.add_event_detect(ENC2[0], GPIO.RISING, callback=rotation_decode2)



    GPIO.setup(throttle, GPIO.OUT)
    GPIO.setup(PWM[0], GPIO.OUT)
    GPIO.setup(PWM[1], GPIO.OUT)
    GPIO.setup(DIR[0], GPIO.OUT)  # set pin as output
    GPIO.setup(DIR[1], GPIO.OUT)  # set pin as output
      

    GPIO.setup(brake_pin, GPIO.OUT)  # set pin as output
    GPIO.setup(reverse_pin, GPIO.OUT)  # set pin as output
    GPIO.setup(N_pin, GPIO.OUT)  # set pin as output

    brake_pwm = [GPIO.PWM(PWM[0], 1000), GPIO.PWM(PWM[1], 1000)]

    throttle_pwm = GPIO.PWM(throttle, 25000)  # create PWM instance with frequency
    GPIO.output(brake_pin, GPIO.HIGH)

#     Data require global
E1_t=0
E2_t=0
throttle_speed = 0
brake_speed = 0
drive_mode = 0
sterring_position = 0
sterring_input = 0
loop_time =0
encoder1_count = 0
encoder2_count = 0
error_encoder1=0
error_encoder2=0
last_error_encoder2 =0
last_error_encoder1 = 0

def rotation_decode2(Enc_B):
    global encoder2_count
    Switch2_A = GPIO.input(ENC2[0])
    Switch2_B = GPIO.input(ENC2[1])
 
    if (Switch2_A == 1) and (Switch2_B == 0):
        encoder2_count -= 1
        #print ("direction -> ", counter)
        return
 
    elif (Switch2_A == 1) and (Switch2_B == 1):
        encoder2_count += 1

    
def rotation_decode1(Enc_A):
    global encoder1_count
    Switch_A = GPIO.input(ENC1[1])
    Switch_B = GPIO.input(ENC1[0])
 
    if (Switch_A == 0) and (Switch_B == 1):
        encoder1_count -= 1
        return
 
    elif (Switch_A == 1) and (Switch_B == 1):
        encoder1_count += 1

    

    
    


def get_data():
    global throttle_speed, brake_speed, drive_mode, sterring_input, sterring_position, mega_connected, flysky_connected, motordriver_connected
    try:
        if ser.in_waiting > 0:
            mega_connected = True
            response = ser.readline().decode('utf-8').rstrip()
            #print(response)
            data = response.split("#")
            #print(len(data))
            if (len(data) == 10):
                if(int(data[1]) >0):
                    throttle_speed = int((int(data[1])-1000)*0.1)
                #print(throttle_speed)
                if(int(data[2]) <= 1500):
                    brake_speed = (1500-int(data[2]))*3
                drive_mode = int(data[3])
                if (drive_mode >= 1900):
                    drive_mode = 'R'
                elif (drive_mode >= 1400 and drive_mode <= 1600):
                    drive_mode = 'F'
                else:
                    drive_mode = 'N'
                sterring_input = int(data[4])
                sterring_position = data[5]

                flysky_connected = True
                mega_connected = True
                motordriver_connected = True




    except:
        mega_connected = False


###################
def drive(drive_speed, drive_mode):
    if (drive_speed >= 100 ):
        drive_speed = (abs(drive_speed) / abs(drive_speed)) * 99
    if(brake_speed > 20):
        drive_speed=0

    if (drive_mode == 'N'):
        GPIO.output(N_pin, GPIO.LOW)
        GPIO.output(reverse_pin, GPIO.HIGH)
        throttle_pwm.start(0)
    elif (drive_mode == 'R'):
        GPIO.output(reverse_pin, GPIO.LOW)
        GPIO.output(N_pin, GPIO.HIGH)
        throttle_pwm.start(drive_speed)
    elif (drive_mode == 'F'):
        GPIO.output(N_pin, GPIO.HIGH)
        GPIO.output(reverse_pin, GPIO.HIGH)
        throttle_pwm.start(drive_speed)
    else:
        GPIO.output(N_pin, GPIO.HIGH)
        GPIO.output(reverse_pin, GPIO.HIGH)
        throttle_pwm.start(0)


def brakeing():
    global error_encoder1,error_encoder2,last_error_encoder2,encoder2_count,encoder1_count,last_error_encoder1,E2_t,E1_t
    error_encoder2 = -(brake_speed + encoder2_count)
    error_encoder1 = -(brake_speed + encoder1_count)

    if (brake_speed > 20):
        GPIO.output(brake_pin, GPIO.LOW)
    else:
        GPIO.output(brake_pin, GPIO.HIGH)

    if (error_encoder1 > 30):
        
        brake_pwm[0].start(100)
        GPIO.output(DIR[0], GPIO.HIGH)
    elif (error_encoder1 < -30):
        brake_pwm[0].start(100)
        GPIO.output(DIR[0], GPIO.LOW)
    else:
        brake_pwm[0].start(0)
        GPIO.output(DIR[0], GPIO.LOW)

    if (error_encoder2 > 30):
        brake_pwm[1].start(100)
        GPIO.output(DIR[1], GPIO.HIGH)
    elif (error_encoder2 < -30):
        brake_pwm[1].start(100)
        GPIO.output(DIR[1], GPIO.LOW)
    else:
        brake_pwm[1].start(0)
        GPIO.output(DIR[1], GPIO.LOW)
    
    
    if(last_error_encoder2 == error_encoder2 and brake_speed == 0 and error_encoder2 != 0 and abs(error_encoder2)>30):
        time.sleep(0.05)
        error_encoder2 = -(brake_speed + encoder2_count)
        if(last_error_encoder2 == error_encoder2 and brake_speed == 0):
            encoder2_count =0
    last_error_encoder2 = error_encoder2
    
    
    if(last_error_encoder1 == error_encoder1 and brake_speed == 0 and error_encoder1 != 0 and abs(error_encoder1) > 30):
        time.sleep(0.05)
        error_encoder1 = -(brake_speed + encoder1_count)
        if(last_error_encoder1 == error_encoder1 and brake_speed == 0):
            encoder1_count =0
    last_error_encoder1 = error_encoder1
    


def stop():
    brake_pwm[0].start(0)
    brake_pwm[0].start(0)
    throttle_pwm.start(0)


def check_error():
    global mega_connected, status_check
    if not (mega_connected):
        status_check = "mega_disconneced"
        # print("mega_disconneced")
        stop()
    elif not (flysky_connected):
        status_check = "flysky_disconneced"
        stop()
        # print("mega_disconneced")
    elif not (motordriver_connected):
        status_check = "motor_driver_disconnected"
        stop()
        # print("motor_driver_disconnected")
    else:
        status_check = "OK"

setup()
while True:
    start_time = time.time()
    get_data()
    drive(throttle_speed, drive_mode)
    brakeing()
    check_error()

    print(encoder1_count,encoder2_count,error_encoder1,error_encoder2)
    print("throttle " + str(throttle_speed) + " brake " + str(brake_speed) + " sterring in " + str(sterring_input)
          + " sterring pos "+str(sterring_position)+" sterring input "+str(sterring_input)+" drive_mode "+
           str(drive_mode) + " loop_time_pi " +str(int(1000*loop_time))+" ms "+ " status " + str(status_check))
    time.sleep(0.005)
    loop_time = time.time() - start_time
