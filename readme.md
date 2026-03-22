# USB Switch Selector

This is a programmable USB 2.0 switch selector that allows two USB devices to connect to a single USB host port, with only one active at a time. Switching is controlled by an external module. USB data and USB power are switched separately, so the selected port gets both the data pair and 5V, while the other port stays disconnected.

The original idea was to use it in a car to switch between two wireless CarPlay adapters with the built-in HomeLink garage opener button.

This device is unidirectional; it cannot switch one USB peripheral between two USB hosts.

![](assets/photo.jpg)

### Files

* _3d\_models_ - files for 3D printing
* _src_ - source code for the controller
* _schematics_ - KiCad project for the board

Check [Releases](releases/latest) for Gerber files, BOM, schematic PDFs, etc.

### Details

[TS3USB30](https://www.digikey.com/en/products/detail/texas-instruments/TS3USB30EDGSR/2193086) switches the USB D+ and D- data lines from a single input to one of two outputs. [LM3526-H](https://www.digikey.com/en/products/detail/texas-instruments/LM3526M-H-NOPB/363943) does the same for the 5V power line. Each USB port has capacitors to help keep the power stable and [USB6B1](https://www.digikey.com/en/products/detail/stmicroelectronics/USB6B1RL/654663) to protect from electrostatic discharge. In addition, there is a [SMF5V0A](https://www.digikey.com/en/products/detail/vishay-general-semiconductor-diodes-division/SMF5V0A-E3-08/1680585) TVS diode on the input port for surge protection.

The board is controlled by [XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html). Pins D7 and D8, D9, D10 (SPI), along with 3.3V and GND, are routed to a 2.54 mm header. In my case, the MCU reads digital signals from the [QIACHIP Remote Switch](https://www.amazon.com/dp/B09P89RF8R), a 433 MHz RF OOK receiver module.

The source code is written in CircuitPython. The switching sequence is: save the new state to a file, turn both power outputs off, disable the USB data switch, select the new side, enable power for that side, enable the USB data switch again, and then update the LEDs and the buzzer.

### Implementation notes

- The R1 and R2 series resistors are 0 Ω links; initial 22 Ω resistors caused connection problems.
- The ferrite beads are recommended in the LM3526 documentation.
- USB data traces should stay short, be routed as a pair, have matched length, and the same number of vias.
- The TS3USB30 enable pin is active low, so a high level means the data path is disconnected.
- There are two variants of the LM3526: -H (active high) and -L (active low).
- The code writes the new state to a file before calling the "off" sequence, because some USB hosts may fully reload when an output port turns off.
- The RP2040 and the USB input share the same 5 V VBUS rail. Do not connect both at the same time, because that may back-power the USB host and damage it.

### License

Hardware design files are licensed under the CERN-OHL-P-2.0 license. Source code files are licensed under the MIT license. This project is provided “as is”, without warranty of any kind.