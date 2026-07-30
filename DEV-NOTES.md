# DEV NOTES

## Board 

Board implementation notes:

* FSUSB63UMX is a 3:1 USB mux. Outputs 1 and 2 are wired to USB ports (controlled by the firmware). Output 3 is for the CH340 programmer (forced by the slide switch).

* TPS2066C controls USB power switching. 

* ATtiny controls data and power switching. Firmware must guarantee that only one output is enabled at a time. 

* Switched VBUS outputs are wired to power-status green LEDs. TPS2066C prevents OUT-to-IN current, but LEDS will get powered if voltage is applied to a USB output port.

* Logical data and power net numbers do not match physical port numbers due to a different pin order.

* USB-C has 5.1k resistors and TVS SMF5V0A surge protection.

* USB input data lines are swapped due to a different pin order in the FSUSB63UMX. The number of VIAs in data lines must match. The total length of tracks in USB data pairs should closely match.

* Each USB input and output has a USBLC6. USBLC6 is a flow-through ESD device, meaning pins 1-6 and 3-4 are connected inside the package. There are custom jumpers in the USBLC6 footprints on the board that connect pins 1-6 and 3-4. However, it worth mentioning that shorted pins 1-6 and 3-4 do work too.

* The slide switch forces FSUSB63UMX to activate the CH340X UPDI programmer. The UPDI pin is reserved exclusively for programming and must not be reused as GPIO. It is connected to the CH340X programming interface and exposed as a troubleshooting test point. Do not apply 12 V to this net because it can damage the CH340X.

* ATtiny 1626 has been considered as an alternative, but it has too many pins and takes too much space.

* ATtiny controls a single red LED to indicate user-defined state.

* ATtiny can be controlled by the active-low push button. The push button has a 2.54mm pin header to connect a wired external button. Since the wire can be long, an extra TVS GSMF3.3A diode, current-limiting resistor, and an external pull-up are added for possible noise and ESD. With two pins, the 2.54 header is physically compatible with thru-hole 2.00mm and 2.50mm JST connectors (confirmed).

* ATtiny can play a tune via CPT-9019A-SMT-TR. It MUST be a piezo transducer, NOT a magnetic buzzer. The selected speaker is compatible with CUI_CPT-9019S-SMT footprint (confirmed).

* The slide 218-2LPST is compatible with Copal_CHS-02B footprint (confirmed).

* ATtiny pins PA1, PA2, PA3, PA4 are wired to 2.54mm extensions as an SPI interface or general GPIO.

* VBUS crosses USB data pairs on the opposite layer at 90 degrees to avoid parallel coupling.

* VBUS lines should switch board layers via two VIAs on each layer (maybe with the exception of small branches).

* 10 µF output capacitance is accepted as an application-specific deviation. 0805 footprint is selected so capacitance can be increased to 22uF or 47uF.


__Validation notes__:

* The implementation notes above must not contradict themselves.

* Schematic and PCB should match the description.

* SPI header is correctly matched to ATtiny SPI hardware.

* BUZZER is connected to a PWM-enabled PIN.

* Final UPDI routing enables simple USB-to-ATtiny Arduino programming.

* Verify USB-C input TVS protection against the diode datasheet.

* Verify the external button TVS protection against the diode datasheet.

# Firmware

TODO
