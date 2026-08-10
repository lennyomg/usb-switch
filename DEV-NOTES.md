# DEV NOTES

## Board 

Board implementation notes:

* FSUSB63UMX is a 3:1 USB mux. Outputs 1 and 2 are wired to USB ports (controlled by the firmware). Output 3 is for the CH340 programmer (forced by the slide switch).

* TPS2066C controls USB power switching. 

* ATtiny controls data and power switching. Firmware must guarantee that only one output is enabled at a time. 

* Switched VBUS outputs are wired to power-status green LEDs. TPS2066C prevents OUT-to-IN current, but LEDs will be powered if voltage is applied to a USB output port.

* Logical data and power net numbers do not match physical port numbers due to a different pin order.

* USB-C has 5.1k resistors and TVS SMF5V0A surge protection.

* USB input data lines are swapped due to a different pin order in the FSUSB63UMX. The number of vias in data lines must match. The total length of tracks in USB data pairs should closely match.

* Each USB input and output has a USBLC6. USBLC6 is a flow-through ESD device, meaning pins 1-6 and 3-4 are connected inside the package. There are custom jumpers in the USBLC6 footprints on the board that connect pins 1-6 and 3-4. However, it is worth mentioning that shorted pins 1-6 and 3-4 work too.

* The 218-2LPST slide in ON-ON position forces FSUSB63UMX to activate the CH340X UPDI programmer. The UPDI pin is reserved exclusively for programming and must not be reused as GPIO. It is connected to the CH340X programming interface and exposed as a troubleshooting test point. Do not apply 12 V to this net because it can damage the CH340X.

* ATtiny 1626 has been considered as an alternative, but it has too many pins and takes too much space.

* ATtiny controls a single red LED to indicate a user-defined state.

* ATtiny can be controlled by the active-low push button. The push button has a 2.54mm pin header to connect a wired external button. Since the wire can be long, an extra TVS GSMF3.3A diode, a current-limiting resistor, and an external pull-up are added for possible noise and ESD. With two pins, the 2.54 header is physically compatible with thru-hole 2.00mm and 2.50mm JST connectors (confirmed).

* ATtiny can play a tune via CPT-9019A-SMT-TR. It MUST be a piezo transducer, NOT a magnetic buzzer. The selected speaker is compatible with the CUI_CPT-9019S-SMT footprint (confirmed).

* At 3.3 V, the ATtiny1624 must be configured for 10 MHz.

* The 218-2LPST slide switch is compatible with the Copal_CHS-02B footprint (confirmed).

* ATtiny pins PA1, PA2, PA3, and PA4 are wired to 2.54mm extensions as an SPI interface or general GPIO.

* Two 47uF 0805 capacitors per an output is a way to mitigate 120uF, recommended by TPS2066C. This is subject to change after testing.

* The board has 4 layers: F.Cu - USB routing and SMD; In1.Cu - GND plane; In2.Cu - power lines and GND plane; B.Cu - control lines.

* USB data trace sizes are calculated to target 90ohm differential impedance for the specified stackup: 0.25mm width, 0.15mm gap, 0.75mm ground plane clearance on the same layer.

* The TPS channels should default OFF during initial attachment. If the MCU immediately enables a channel, the upstream source will experience the input capacitor AND output capacitors. The TPS soft-start might mitigate that, but avoiding immediate enable is better.

* USB C resistors must be 5.1k +/-1%.

* USB shield is connected to GND.

__Validation notes__:

* The implementation notes above must not contradict themselves.

* Schematic and PCB should match the description.

* SPI header is correctly matched to ATtiny SPI hardware.

* BUZZER is connected to a PWM-enabled PIN.

* Final UPDI routing enables simple USB-to-ATtiny Arduino programming.

* Verify USB-C input TVS protection against the diode datasheet.

* Verify the external button TVS surge protection against the diode datasheet.

* The USB-C routing must be valid for a USB-C peripheral device.

* Verify capacitors and resistors values and accuracy.

# Firmware

TODO
