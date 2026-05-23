import board
from time import sleep
from pwmio import PWMOut
from adafruit_debouncer import Button
from digitalio import DigitalInOut, Pull

BTN0 = board.A0
EXT1 = board.MOSI
EXT2 = board.MISO
EXT3 = board.SCK
EXT4 = board.RX


buzzer = PWMOut(board.A1, variable_frequency=True)

pwr_a = DigitalInOut(board.D2)  # right port
pwr_a.switch_to_output(value=False)
pwr_b = DigitalInOut(board.D3)  # left port
pwr_b.switch_to_output(value=False)
usb_en = DigitalInOut(board.D4)
usb_en.switch_to_output(value=False)  # 'False' selects port A, 'True' selects port B
usb_sel = DigitalInOut(board.D5)
usb_sel.switch_to_output(value=True)  # 'True' means disabled.

btn0_pin = DigitalInOut(BTN0)
btn0_pin.switch_to_input(pull=Pull.UP)
ext1_pin = DigitalInOut(EXT1)
ext1_pin.switch_to_input(pull=Pull.UP)

btn0 = Button(btn0_pin)
ext1 = Button(ext1_pin)

def off():
    pwr_a.value = False
    pwr_b.value = False
    usb_en.value = True
    sleep(0.1)


def switch_to_a():
    usb_sel.value = False
    usb_en.value = False
    sleep(0.1)
    pwr_a.value = True
    beep(660, 0.1)
    beep(440, 0.1)
    beep(660, 0.1)


def switch_to_b():
    usb_sel.value = True
    usb_en.value = False
    sleep(0.1)
    pwr_b.value = True
    beep(440, 0.15)
    beep(660, 0.15)


def beep(frequency, duration):
    buzzer.frequency = frequency
    buzzer.duty_cycle = 32768
    sleep(duration)
    buzzer.duty_cycle = 0


def write_state(state: int):
    try:
        with open("/state.txt", "w") as f:
            f.write(str(state))
            f.flush()
    except Exception as e:
        print("error writing state:", e)


def read_state() -> int:
    try:
        with open("/state.txt", "r") as f:
            state = int(f.read())
            return state
    except Exception as e:
        print("error reading state:", e)
        return 0


state = read_state()

if state == 1:
    switch_to_a()

if state == 2:
    switch_to_b()

while True:

    btn0.update()
    ext1.update()
    pressed = btn0.pressed or ext1.pressed

    # some usb hosts may turn power off and do full reload if one of ports has been turned off
    # save all data before 'off()'

    if pressed:
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
