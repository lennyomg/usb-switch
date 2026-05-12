import board
import digitalio
import storage

try:
    neopixel_pwd = digitalio.DigitalInOut(board.NEOPIXEL_POWER)
    neopixel_pwd.switch_to_output(value=False)
except AttributeError:
    pass        

storage.remount("/", readonly=False)

# to make the CP drive appear on the host
# connect via terminal, press Ctrl-C to stop main code, press Enter to enter REPL
# >>> import os
# >>> os.remove("boot.py")
# reconnect the board