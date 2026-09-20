# USB Switch

This is a programmable USB 2.0 switch that connects one of two USB devices to a USB host at a time. It switches both data and power, fully disconnecting the inactive device. You can control it with a button or an external module.

This device is not a USB hub, neither logically nor internally. The switch is unidirectional: it cannot switch a single USB peripheral between two USB hosts.

### Technical details

The board is powered by an ATtiny1624, which controls FSUSB63UMX for switching USB data and TPS2066C for switching USB power. The board includes:
* 16 KB of storage, 2 KB of memory, and 256 bytes of EEPROM
* a 3.3 V regulator with a maximum current of 250 mA
* a USB-to-serial converter
* a single accessible LED
* two non-accessible LEDs for power status
* a built-in push button with an external 2.54 mm header
* a piezo speaker
* 2.54 mm header with 3.3 V, GND, and four GPIO/SPI pins

Check the [assembly guide](ASSEMBLY-GUIDE.md) and [dev notes](DEV-NOTES.md). Gerber files, BOM, STLs, schematic can be found in [releases](https://github.com/lennyomg/usb-switch/releases/latest).

### Application

This is primarily designed as a wired or wireless remote control for USB switching. I added an external [433MHz radio garage door opener receiver](https://www.amazon.com/dp/B09P89RF8R) module and now I can switch between two wireless CarPlay adapters using my car HomeLink garage opener button.

### License

Hardware design files are licensed under the CERN-OHL-P-2.0 license. Source code files are licensed under the MIT license. This project is provided “as is”, without warranty of any kind.
