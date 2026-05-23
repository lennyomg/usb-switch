import board
import storage
from digitalio import DigitalInOut, Pull

try:
    neopixel_pwd = DigitalInOut(board.NEOPIXEL_POWER)
    neopixel_pwd.switch_to_output(value=False)
except AttributeError:
    pass        

# hold the button while powering on to mount drive as writable

btn = DigitalInOut(board.A0)
btn.switch_to_input(pull=Pull.UP)
if btn.value: # not pressed
    storage.remount("/", readonly=False)
