import board
import digitalio
import time
import pwmio
import adafruit_debouncer

# A is the right port, B is the left port
# A is blue, B is green

led_a = digitalio.DigitalInOut(board.LED_BLUE)
led_a.switch_to_output(value=False)

led_b = digitalio.DigitalInOut(board.LED_GREEN)
led_b.switch_to_output(value=False)

buzzer = pwmio.PWMOut(board.A1, variable_frequency=True)

rf_pin = digitalio.DigitalInOut(board.D10)
rf_pin.switch_to_input(pull=digitalio.Pull.UP)
rf = adafruit_debouncer.Button(rf_pin)

pwr_a = digitalio.DigitalInOut(board.D2)
pwr_a.switch_to_output(value=False)

pwr_b = digitalio.DigitalInOut(board.D3)
pwr_b.switch_to_output(value=False)

usb_en = digitalio.DigitalInOut(board.D4)
usb_en.switch_to_output(value=False)

usb_sel = digitalio.DigitalInOut(board.D5)
usb_sel.switch_to_output(value=True)


def beep(frequency, duration):
    buzzer.frequency = frequency
    buzzer.duty_cycle = 32768
    time.sleep(duration)
    buzzer.duty_cycle = 0
    pass


def write_state(state: int):
    try:
        with open("/state.txt", "w") as f:
            f.write(str(state))
            f.flush()
    except Exception as e:
        print("error writing state:", e)


def off():
    pwr_a.value = False
    pwr_b.value = False
    usb_en.value = True
    led_a.value = True
    led_b.value = True
    time.sleep(0.1)


def switch_to_a():
    usb_sel.value = False
    time.sleep(0.5)
    pwr_a.value = True
    time.sleep(0.5)
    usb_en.value = False
    time.sleep(0.5)
    led_a.value = False
    led_b.value = True
    beep(660, 0.1)
    beep(440, 0.1)
    beep(660, 0.1)


def switch_to_b():
    usb_sel.value = True
    time.sleep(0.5)
    pwr_b.value = True
    time.sleep(0.5)
    usb_en.value = False
    time.sleep(0.5)
    led_a.value = True
    led_b.value = False
    beep(440, 0.15)
    beep(660, 0.15)


state = 0
last_change = time.monotonic()

beep(880, 0.1)

try:
    with open("/state.txt", "r") as f:
        state = int(f.read())
except Exception as e:
    print("error reading state:", e)
    state = 0

if state == 1:
    switch_to_a()

if state == 2:
    switch_to_b()

while True:

    # some usb hosts may turn power off and do full reload if one of ports has been turned off
    # save all data before 'off()' is called

    rf.update()
    if rf.pressed:
        last_change = time.monotonic()
        if state == 1:
            state = 2
            write_state(state)
            off()
            switch_to_b()
        else:
            state = 1
            write_state(state)
            off()
            switch_to_a()

    if last_change != 0 and time.monotonic() - last_change > 60: # 60 sec
        led_a.value = True
        led_b.value = True
        last_change = 0


