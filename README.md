# Daly2Sofar v0.9 - beta
ESP32 bridge allowing Daly Smart BMS to be used with a Sofar inverter/charger (and others that use SMA CANBUS protocol).
![Daly2Sofar_1](https://user-images.githubusercontent.com/43951291/164450862-89615694-7e51-4a53-a4ad-102c156572c5.jpg)



It connects to Daly via UART and transmits to the inverter on CANBUS (SMA protocol).
At the same time, it will transmit the BMS data to an MQTT broker if it can connect to your WiFi.
If it can't connect to WiFi or your MQTT broker, it will still work as a BMS-to-Inverter bridge.
The icons on the OLED represent the WiFi & MQTT status. They will be crossed out if there is no connection.

Data comms is one way only - data is requested from Daly and sent to the Inverter & over MQTT.
May be compatible with other inverters that use the SMA protocol.

My testing showed that Daly UART port works on 3.3v. PLEASE measure yours before connecting the ESP. Some users report 5V.

![Daly2Sofar_2](https://user-images.githubusercontent.com/43951291/164450885-7a7a2b6e-d87b-47af-a21a-84e1c9d92381.jpg)
![Daly2Sofar_3](https://user-images.githubusercontent.com/43951291/164450894-d7f85e7a-e5d1-40e6-8250-39d048bfd524.jpg)



# PSA:
USE THIS AT YOUR OWN RISK!
Batteries are dangerous. Don't come to me if you burn your house down.
This release is a working prototype. It might freeze, there might be glitches. Your battery might overcharge/undercharge. Make sure your BMS is set up properly with your own limits.
USE THIS AT YOUR OWN RISK!

# Schematic
![Daly2Sofar schematic](https://user-images.githubusercontent.com/43951291/164440895-b26a7267-7dd1-4e10-8e55-49fe68698d4c.jpg)


# Isolation
Some battery setups earth the battery negative, others don't.
For the ones that aren't earthed, you may get a ID05 Fault on the inverter when you connect canbus.
You might get a shock if you touch the comms/3v3/ground pins coming from the BMS, even though they're supposed to be only 3.3v.

Ask me how I know!

This is because the Daly UART ground is battery ground, and it makes its way to the inverter.
It might be higher or lower voltage potential, in respect to earth.

The Daly needs to be isolated in this case, using a digital isolation IC.
Opto-coupler's don't work as the signal becomes inverted. ESP32 serial can be inverted by software but Daly can't.
I used a IL716-3E isolator IC and soldered dupont pins to it's legs - it's tiny, it's not easy!
If you find an isolator IC module that is cheap and works without soldering, please let me know!


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


# MQTT info:
Add your WiFi and MQTT details to the top of the sketch, as well as your battery capacity and preferred charging voltage (Capacity is broken at the moment, but the Inverter seems happy nonetheless):

![image](https://user-images.githubusercontent.com/43951291/164454052-098721e5-b45f-4cef-878d-7d6928e60f59.png)

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

# Node-RED
For extracting the data into Home Asisstant sensors, use Node-Red. You can import the ```Daly2Sofar Node-Red to Home Assistant flow.json```flow to extract the data into sensor entities, it will look like this:
![Daly2Sofar Node-Red to HA example](https://user-images.githubusercontent.com/43951291/164437077-1af12d01-b174-4be1-bb78-2a273e6da55a.jpg)
(you may need to install Moving Average filter if you want smoothing of data)

Home Assistant dashboard example
![Home Assistant sensors example](https://user-images.githubusercontent.com/43951291/164439575-7c585c8e-12b4-412a-9ef5-60dd962c2ea7.jpg)

When the battery is discharging, Node-RED will calculate the remaining battery time based on the current discharge rate.
This has some hard-coded variables - it's based on a 10kWh battery and 80% DOD. Make sure you customise them if you want to use this Flow :-)
![image](https://user-images.githubusercontent.com/43951291/164440385-f34659a5-c4ad-48e3-82c1-d9a69adf85bb.png)

![image](https://user-images.githubusercontent.com/43951291/164440193-95620cbc-c11d-4a18-8acf-261b9363472a.png)


# To do:
- Fix battery capacity.
- Trace all the dependencies and link them.
- Need to figure out how to obtain battery cycles from BMS.
- Tidy up code/refactor where poss.
- Longer term stability testing.

# Arduino dependencies

https://github.com/maland16/daly-bms-uart

https://github.com/miwagner/ESP32-Arduino-CAN

https://github.com/me-no-dev/AsyncTCP

https://github.com/marvinroger/async-mqtt-client

# Notes/links

Using SMA_CAN_protocol.pdf for the CAN IDs and data format info (google will find you the PDF)

Using https://cryptii.com/pipes/integer-encoder for working out hex values

VIEW TEXT > DECODE INTEGER (LITTLE ENDIAN) U16 > VIEW BYTES HEX GROUPED BY BYTES

https://www.scadacore.com/tools/programming-calculators/online-hex-converter/ for checking hex values
