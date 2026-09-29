# DEV NOTES

## Firmware

* Firmware source code is for Platform IO.

* Follow the assembly guide to install USB drivers. Update `upload_port` in `platformio.ini` accordingly.

* Set the switch to the ON-ON position to force the board into development mode and activate the built-in USB programmer.

* Firmware must guarantee that only one output is enabled at a time.

* `PIN_USB_PWR_1` and `PIN_USB_PWR_2` are intentionally swapped to compensate for the corresponding swap on the board.

* Some USB hosts may briefly cut off power when the board turns off both outputs. Therefore, save any persistent state before switching outputs.

* Each EEPROM cell has a limited write endurance (approximately 100,000 cycles), so depending on usage, consider rotating addresses used for stored data.

* On switching, disconnect data before disabling VBUS. When enabling a port, enable VBUS first and connect data after VBUS has stabilized. Keep the pre-connect powered interval below 100 ms.

* Set pins PA1, PA2, PA3, and PA4 to `INPUT_PULLUP` mode unless they are used by an external IC.

* The firmware must be configured for 10 MHz and internal oscillator.

## Board

* FSUSB63UMX is a 3:1 USB multiplexer. Outputs 1 and 2 are wired to USB ports and controlled by firmware. Output 3 is connected to CH340 programmer and selected by the slide switch.

* TPS2066C controls USB power switching.

* ATtiny controls data and power switching.

* Switched VBUS outputs are wired to green power-status LEDs. TPS2066C prevents OUT-to-IN current, but LEDs will be powered if voltage is applied to a USB output port.

* Logical data and power net numbers do not match physical port numbers because of the different pin order.

* The USB-C connection has 5.1 kΩ resistors and SMF5V0A TVS surge protection.

* USB input data lines cross each other on different layers because of FSUSB63UMX's pin order. The number of vias on each data line must match, and traces within each USB differential pair should be closely length-matched.

* Each USB input and output has a USBLC6. USBLC6 is a flow-through ESD protection device, meaning that pins 1-6 and 3-4 are connected internally. The board's custom USBLC6 footprints include jumpers that connect these pin pairs, although directly shorting each pair also works.

* Setting 218-2LPST slide switch to the ON-ON position forces FSUSB63UMX to activate CH340X UPDI programmer. The UPDI pin is reserved exclusively for programming and must not be reused as GPIO. It is connected to CH340X programming interface and exposed as a troubleshooting test point. Do not apply 12 V to this net, as doing so could damage CH340X.

* ATtiny controls a single red LED to indicate a user-defined state.

* ATtiny can be controlled by the active-low push button. The push button has a 2.54 mm pin header for connecting a wired external button. Because the wire may be long, a GSMF3.3A TVS diode, a current-limiting resistor, and an external pull-up are included to protect against noise and ESD. The two-pin 2.54 mm header is physically compatible with through-hole 2.00 mm and 2.50 mm JST connectors (confirmed).

* ATtiny can play a tune through a CPT-9019A-SMT-TR. It MUST be a piezoelectric transducer, NOT a magnetic buzzer. The selected transducer is compatible with `CUI_CPT-9019S-SMT` footprint (confirmed).

* 218-2LPST slide switch is compatible with Copal_CHS-02B footprint (confirmed).

* ATtiny pins PA1, PA2, PA3, and PA4 are connected to 2.54 mm headers to use as an SPI interface or GPIO.

* Two 47 µF 0805 capacitors per an output is a way to mitigate the recommended 120 µF (TPS2066C specs).

* The board has four layers: F.Cu for USB routing and SMD; In1.Cu for the GND plane; In2.Cu for power lines and the GND plane; and B.Cu for control lines.

* USB data trace dimensions target a 90 Ω differential impedance for the specified stackup: 0.25 mm width, 0.15 mm gap, and 0.75 mm clearance from the ground plane on the same layer.

* TPS channels should default to OFF during initial attachment. If the MCU immediately enables a channel, the upstream source will see both input and output capacitance. The TPS soft-start function might mitigate this, but it is better to avoid enabling a channel immediately.

* USB-C resistors must be 5.1 kΩ **±1%**.

* USB shields are connected to GND.

## Pre-order Validation

* Implementation notes above must not contradict themselves.

* Schematic and PCB should match the description.

* SPI header is correctly mapped to ATtiny's SPI hardware.

* Buzzer is connected to a PWM-capable pin.

* Final UPDI routing enables simple USB-to-ATtiny Arduino programming.

* Verify USB-C input TVS protection against the diode datasheet.

* Verify the external button TVS surge protection against the diode datasheet.

* USB-C routing must be valid for a USB-C peripheral device.

* Verify capacitor and resistor values and tolerances.
