import blynklib
import random
import time
import RPi.GPIO as GPIO
import serial
import Encoder

enc = Encoder.Encoder(18, 17)  # (DT,CLK)
enc2 = Encoder.Encoder(19, 26)

# ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1) #9600 is baud rate(must be same with that of NodeMCU)
GPIO.setwarnings(False)

GPIO.setmode(GPIO.BCM)

BLYNK_AUTH = 'QC5A5UnfP06tRzkqA5sVxkpnMIrXc2i2'
blynk = blynklib.Blynk(BLYNK_AUTH)

throttle = 12  # PWM pin connected to Throttle wire
brack_pin = 11
reverse_pin = 10
N_pin = 9
PWM1 = 23
DIR1 = 24
PWM2 = 20
DIR2 = 21
GPIO.setup(throttle, GPIO.OUT)
GPIO.setup(PWM1, GPIO.OUT)
GPIO.setup(PWM2, GPIO.OUT)
GPIO.setup(DIR1, GPIO.OUT)  # set pin as output
GPIO.setup(DIR2, GPIO.OUT)  # set pin as output
GPIO.setup(brack_pin, GPIO.OUT)  # set pin as output
GPIO.setup(reverse_pin, GPIO.OUT)  # set pin as output
GPIO.setup(N_pin, GPIO.OUT)  # set pin as output

brack1_pwm = GPIO.PWM(PWM1, 10)
brack2_pwm = GPIO.PWM(PWM2, 10)

pi_pwm = GPIO.PWM(throttle, 10000)  # create PWM instance with frequency

val = 0
reverse = 0
current_value = 0
drive_val = 0


def valueChanged(value, direction):
    val = int(value)
    if direction == "R":
        stp.step_run(1, 100)
        print("left")
    elif direction == "L":
        stp.step_run(-1, 100)
        print("right")


def tell(msg):
    msg = msg + '\n'
    x = msg.encode('ascii')  # encode n send
    ser.write(x)
    print(x)


def hear():
    line = ser.readline().decode('utf-8').rstrip()
    print("received: ", line)


def drive(val):
    Speed = [val]
    for spd in Speed:
        if abs(spd) > 99:
            spd = (abs(spd) / spd) * 99
        if spd > 0:

            pi_pwm.start(abs(spd))
            # sleep(0.05)
        else:
            # pi_pwm.start(abs(spd))
            pi_pwm.start(0)
            # sleep(0.05)


def brack(val):
    Speed = [val]
    for spd in Speed:
        if abs(spd) > 99:
            spd = (abs(spd) / spd) * 99
        if spd > 0:
            brack1_pwm.start(100)
            GPIO.output(DIR1, GPIO.LOW)
            brack2_pwm.start(100)
            GPIO.output(DIR2, GPIO.LOW)

            # sleep(0.05)
        if spd < 0:
            # pi_pwm.start(abs(spd))
            brack1_pwm.start(100)
            GPIO.output(DIR1, GPIO.HIGH)
            brack2_pwm.start(100)
            GPIO.output(DIR2, GPIO.HIGH)
            # sleep(0.05)
        if spd == 0:
            brack1_pwm.start(0)
            brack2_pwm.start(0)


# register handler for virtual pin V4 write event
@blynk.handle_event('write V5')
def v5_pin_handler(pin, value):
    tell(value[0])


@blynk.handle_event('write V0')
def v0_pin_handler(pin, value):
    global drive_val
    drive_val = int(value[0])
    # print(WRITE_EVENT_PRINT_MSG.format(pin, value))


@blynk.handle_event('write V4')
def v4_pin_handler(pin, value):
    # brack(int(value[0]))
    global val
    val = int(value[0])


@blynk.handle_event('write V2')
def v2_pin_handler(pin, value):
    # brack(int(value[0]))
    reverse = 0
    reverse = int(value[0])

    if reverse == 1:
        GPIO.output(reverse_pin, GPIO.HIGH)
    else:
        GPIO.output(reverse_pin, GPIO.LOW)


@blynk.handle_event('write V3')
def v3_pin_handler(pin, value):
    # brack(int(value[0]))
    neutral = 0
    neutral = int(value[0])

    if neutral == 1:
        GPIO.output(N_pin, GPIO.HIGH)
    else:
        GPIO.output(N_pin, GPIO.LOW)


def bracking(val, encoder1, encoder2):
    global current_value
    if (val > 0):
        # GPIO.output(brack_pin, GPIO.HIGH)
        if (encoder1 >= val * 3.3 and encoder2 >= val * 3.3):
            brack(0)
        else:
            current_value = val
            brack(val)

    elif (val == 0) and (encoder1 > 0) and encoder2 > 0:
        # GPIO.output(brack_pin, GPIO.HIGH)
        if (encoder1 == 0):
            brack(0)
        else:
            brack(-encoder1)
    elif (val == 0 and encoder1 < 0 and encoder2 < 0):
        # GPIO.output(brack_pin, GPIO.HIGH)
        if (encoder1 == 0):
            brack(0)
        else:
            brack(encoder1)


###########################################################
# infinite loop that waits for event
###########################################################
while True:
    blynk.run()  # 330 for 25mm linear distance
    drive(drive_val)
    # print(drive_value)
    encoder1 = enc.read()
    encoder2 = enc2.read()
    bracking(val, encoder1, encoder2)
    if (val > 40):
        GPIO.output(brack_pin, GPIO.LOW)
    else:
        GPIO.output(brack_pin, GPIO.HIGH)
    # print(encoder1,encoder2,val)