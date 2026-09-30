# Assembly guide

This is how to build, print, flash, and assemble this project. Check [releases](https://github.com/lennyomg/usb-switch/releases/latest) for manufacturing files.

<img width="1500" height="1125" alt="image" src="https://github.com/user-attachments/assets/844c571f-c0c6-4f61-b146-436f7a7543de" />


## Board

Order a 4-layer PCB using the attached Gerber and drill files. Add a stencil to your order if possible. A rework station or hot plate is required to assemble the board, the USB mux is too small for a soldering iron.

The BOM contains manufacturer part numbers for DigiKey and LCSC. When ordering parts, pay attention to the specified tolerances (5.1 kΩ **±1%** resistors, for example).

### Enclosure

[Project in OnShape](https://cad.onshape.com/documents/5c8a980f8b2f233ca52af940/w/8f21e194ecc758069b76b15f/e/5b9ec8950a9bfdcf718e2e62).

The project has configuration parameters to increase the box width and height. It also has options to hide the button, speaker holes, and LEDs.

Print in regular PETG with a **0.2 mm** nozzle, preferably in a dark color. Cut small pieces of translucent PETG filament and insert them into the LED holes to make light diffusers. Screw the top and bottom parts together with four M2 screws.

## USB serial driver

The board includes CH340 USB-to-serial converter for programming. Set the onboard switch to the **ON-ON** position, disconnect all devices from the USB-A outputs, and connect the board's USB-C input to the computer.

##### macOS

Install the WCH driver with Homebrew:

```sh
brew install --cask wch-ch34x-usb-serial-driver
```

Follow any prompt to allow the driver in **System Settings > Privacy & Security**. Restart the computer if requested, reconnect the board, and run `ls /dev/cu.*` again. Look for a device such as `/dev/cu.wchusbserial1420` or `/dev/cu.usbserial-1420`.

##### Windows

Windows may install the CH340 driver automatically. To find the port, open **Device Manager > Ports (COM & LPT)**, look for **USB-SERIAL CH340**, and note its port, such as `COM7`.

If the device is missing or appears with a warning icon, download and install the official [WCH CH340/CH341 Windows driver](https://www.wch-ic.com/downloads/CH341SER_ZIP.html). 

## Install AVRDUDE

Use AVRDUDE 8 or newer for flashing the device.

##### macOS

Install AVRDUDE with Homebrew:

```sh
brew install avrdude
```

##### Windows

1. Download the latest Windows x64 ZIP archive from the official [AVRDUDE releases page](https://github.com/avrdudes/avrdude/releases/latest).
2. Extract the archive to a permanent folder.
3. Add that folder to `PATH`, or open PowerShell in the extracted folder and run `avrdude.exe` from there.

## Flash the board

Replace the example serial port and `file.hex` with the actual port and firmware file path.

##### macOS

```sh
avrdude -c serialupdi -p t1624 -P /dev/cu.wchusbserial1420 -b 115200 -U flash:w:file.hex:i
```

##### Windows

```powershell
.\avrdude.exe -c serialupdi -p t1624 -P COM7 -b 115200 -U flash:w:file.hex:i
```

Wait for AVRDUDE to report that verification succeeded. Disconnect the USB-C cable, set the onboard switch to the **1-2** position, and reconnect the board.
