import RPi.GPIO as GPIO
from time import sleep
import time
 
counter = 0
counter2 = 0
dta =0
dtb =0
Enc_A = 18  
Enc_B = 23
Enc2_A = 3
Enc2_B = 2

abs_count= 0
 
def init():
    print ("Rotary Encoder Test Program")
    GPIO.setwarnings(True)
    GPIO.setmode(GPIO.BCM)
    
    GPIO.setup(Enc_A, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(Enc_B, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(Enc2_A, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(Enc2_B, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    
    GPIO.add_event_detect(Enc_A, GPIO.RISING, callback=rotation_decode)
    GPIO.add_event_detect(Enc2_A, GPIO.RISING, callback=rotation_decode2)

    #GPIO.add_event_detect(Enc_B, GPIO.RISING, callback=rotation_decode, bouncetime=10)

    return
 
 
def rotation_decode(Enc_A):
    global counter,dta,dtb,abs_count
    #print(Enc_A)
    Switch_A = GPIO.input(18)
    Switch_B = GPIO.input(23)
    
    if (Switch_A == 1) and (Switch_B == 0):
        counter += 1

 
    elif (Switch_A == 1) and (Switch_B == 1):
        counter -= 1

    

def rotation_decode2(Enc2_A):
    global counter2
    #print(Enc_A)
    Switch2_A = GPIO.input(2)
    Switch2_B = GPIO.input(3)
    
    if (Switch2_A == 0) and (Switch2_B == 1):
        counter2 += 1

 
    elif (Switch2_A == 1) and (Switch2_B == 1):
        counter2 -= 1

    
    
 
def main():
    try:
        init()
        while True :
            print(counter,counter2)
            time.sleep(0.1)
    except KeyboardInterrupt:
        GPIO.cleanup()
 
if __name__ == '__main__':
    main()