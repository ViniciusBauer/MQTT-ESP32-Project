# MQTT Publisher

This folder contains the code for the MQTT Publisher module of the project.

## Description
The publisher is implemented using an ESP32 microcontroller. It connects to a Wi-Fi network and publishes simulated battery data (voltage, current, and temperature) to an MQTT broker at regular intervals. This data can be consumed by a subscriber for further processing.

## How it Works
1. The ESP32 connects to a Wi-Fi network using the credentials provided in the code.
2. It establishes a connection to an MQTT broker (e.g., `test.mosquitto.org`).
3. The module simulates battery metrics:
   - **Voltage**: Between 3.0V and 4.2V.
   - **Current**: Between 0.0A and 5.0A.
   - **Temperature**: Between 20.0°C and 45.0°C.
4. These metrics are formatted into a payload and published to the MQTT topic `battery/data` at 5-second intervals.
5. If the connection to the broker is lost, the module automatically attempts to reconnect.

## Requirements
- **Hardware**: ESP32 microcontroller.
- **Libraries**:
  - `WiFi.h`: For Wi-Fi connection.
  - `PubSubClient.h`: For MQTT communication.

## MQTT Configuration
- **Broker**: `test.mosquitto.org`
- **Port**: `1883`
- **Topic**: `battery/data`

## How to Use
1. Open the code in an Arduino IDE or PlatformIO.
2. Update the Wi-Fi credentials (`ssid` and `password`) with your network details.
3. Upload the code to the ESP32.
4. Monitor the serial output to verify that data is being published to the MQTT broker.
