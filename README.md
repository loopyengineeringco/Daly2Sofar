# Daly2Sofar v0.9 - beta
ESP32 bridge allowing Daly Smart BMS to be used with a Sofar inverter/charger (and others that use SMA CANBUS protocol).

It connects to Daly via UART and transmits to the inverter on CANBUS (SMA protocol).
At the same time, it will transmit the BMS data to an MQTT broker if it can connect to your WiFi.
If it can't connect to WiFi or your MQTT broker, it will still work as a BMS-to-Inverter bridge.

Data comms is one way only - data is requested from Daly and sent to the Inverter & over MQTT.
May be compatible with other inverters that use the SMA protocol.

My testing showed that Daly UART port works on 3.3v. PLEASE measure yours before connecting the ESP. Some users report 5V.

# Schematic
![Daly2Sofar schematic](https://user-images.githubusercontent.com/43951291/164433391-5f333955-79c3-4bfa-9f3e-f2f51964ec53.jpg)



# Isolation
Some battery setups earth the battery negative, others don't.
For the ones that aren't earthed, you may get a ID05 Fault on the inverter when you connect canbus.
This is because the Daly UART ground is battery ground, and it makes its way to the inverter.
The Daly needs to be isolated in this case, using a digital isolation IC.
Opto-coupler's don't work as the signal becomes inverted. ESP32 serial can be inverted by software but Daly can't.
I used a IL716-3E isolator IC and soldered dupont pins to it's legs - it's tiny, it's not easy!
If you find an isolator IC module that is cheap and works without soldering, please let me know!
******

Using SMA_CAN_protocol.pdf for the CAN IDs and data format info (google will find you the PDF)
Using https://cryptii.com/pipes/integer-encoder for working out hex values
VIEW TEXT > DECODE INTEGER (LITTLE ENDIAN) U16 > VIEW BYTES HEX GROUPED BY BYTES
https://www.scadacore.com/tools/programming-calculators/online-hex-converter/ for checking hex values


# ESP32 pin remapping
Make sure the I2C pins are mapped correctly for the Oled to work. For me, pins 21 and 22 work great
C:\Users\XXXXX\Documents\Arduino\libraries\Adafruit_SSD1306-1.1.2\Adafruit_SSD1306.cpp
```
#define I2C_SDA 21
#define I2C_SCL 22
```

And also Serial2 needs to be correctly mapped in this file (This is Windows. Sorry I don't know where it is on a Mac)
C:\Users\XXXXX\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.6\cores\esp32\HardwareSerial.cpp
Look for this near the top:
```
#ifndef RX2
#define RX2 16
#endif

#ifndef TX2
#define TX2 17
#endif
```
******


# MQTT info:
Subscribe your MQTT client to:
```Daly2Sofar/state```

topics published are:
```
soc
voltage
current
power
temp
lowestcell
highestcell
cellimbalance
```
For extracting the data into Home Asisstant sensors, use Node-Red. Have a look at the github for an example Node-Red setup.

# To do:
- Need to figure out how to obtain battery cycles from BMS.
- Tidy up code/refactor where poss.
- Longer term stability testing.
