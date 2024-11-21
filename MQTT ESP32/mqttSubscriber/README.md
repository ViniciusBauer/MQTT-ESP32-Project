# MQTT Subscriber

This folder contains the code for the MQTT Subscriber module of the project.

## Description
The subscriber is implemented using an ESP32 microcontroller. It connects to a Wi-Fi network and an MQTT broker, subscribes to the `battery/data` topic, and processes the incoming messages. The received messages are expected to include simulated battery data such as MAC address, voltage, current, and temperature.

## How it Works
1. The ESP32 connects to a Wi-Fi network using the credentials provided in the code.
2. It establishes a connection to an MQTT broker (e.g., `test.mosquitto.org`).
3. The module subscribes to the topic `battery/data` and listens for incoming messages.
4. Upon receiving a message, it extracts and parses the data:
   - **MAC Address**: Identifies the device sending the data.
   - **Voltage**: Represents the battery voltage.
   - **Current**: Represents the battery current.
   - **Temperature**: Represents the battery temperature.
5. The extracted data is displayed on the serial monitor for further analysis.

## Requirements
- **Hardware**: ESP32 microcontroller.
- **Libraries**:
  - `WiFi.h`: For Wi-Fi connection.
  - `PubSubClient.h`: For MQTT communication.

## MQTT Configuration
- **Broker**: `test.mosquitto.org`
- **Port**: `1883`
- **Subscribed Topic**: `battery/data`

## How to Use
1. Open the code in an Arduino IDE or PlatformIO.
2. Update the Wi-Fi credentials (`ssid` and `password`) with your network details.
3. Upload the code to the ESP32.
4. Monitor the serial output to verify that data is being received and processed.

## Notes
- The payload format expected by the subscriber is:
MAC: XX:XX:XX:XX:XX:XX, Voltage: X.XXV, Current: X.XXXA, Temperature: XX.X°C

Ensure the publisher sends data in this format.
