// -----------------------------------------------------------------------------------------------------------------------
// Program Module: MQTT Publisher Test
// -----------------------------------------------------------------------------------------------------------------------
// V. Bauer       
// -----------------------------------------------------------------------------------------------------------------------
// Description:
// This program demonstrates how to set up an ESP32 to connect to a Wi-Fi and an MQTT broker,
// sendind simulated battery dada (voltage, current, and temperature) at regular intervals.
// -----------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------
// Libraries

#include <Arduino.h>
#include <WiFi.h>                           // Library for Wi-Fi connection
#include <PubSubClient.h>                   // Library for MQTT communication
// #include "freertos/FreeRTOS.h"              
// #include "freertos/task.h"                  
// #include "freertos/queue.h"                 

// -----------------------------------------------------------------------------------------------------------------------

// Variables for time control
unsigned long lastSendTime = 0;                // Stores the timestamp of the last message sent
const unsigned long sendInterval = 5000;       // Interval between messages (in milliseconds)

// -----------------------------------------------------------------------------------------------------------------------
// Hardware mapping

// -----------------------------------------------------------------------------------------------------------------------
// Configuration

// Wi-Fi Configuration
const char* ssid = "ssid";                            // Wi-Fi network name (SSID)
const char* password = "password";                    // Wi-Fi password

// MQTT Configuration
const char* mqttServer = "test.mosquitto.org";        // MQTT broker address
const int mqttPort = 1883;                            // MQTT broker port
const char* mqttUser = "mqqtUser";                    // (Optional) MQTT username
const char* mqttPassword = "mqttPassword";            // (Optional) MQTT password

// -----------------------------------------------------------------------------------------------------------------------
// Global Objects and Variables

String macAddress;                                    // Variable to store ESP32 MAC Address
WiFiClient espClient;                                 // Object to manage Wi-Fi connection
PubSubClient client(espClient);                       // Object to manage MQTT connection

// -----------------------------------------------------------------------------------------------------------------------
// Function Prototypes

void setupWiFi();                                     // Initialize Wi-Fi connection
void connectMQTT();                                   // Connect to the MQTT broker
void sendBatteryData();                               // Simulate and send battery data via MQTT

// -----------------------------------------------------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);                               // Initialize serial communication
    setupWiFi();                                        // Set up Wi-Fi connection
    client.setServer(mqttServer, mqttPort);             // Configure the MQTT broker
}

// -----------------------------------------------------------------------------------------------------------------------

void loop() {
  // Check and maintain the connection to the MQTT broker
  if (!client.connected()) {
    connectMQTT();
  }
  // Keep the MQTT connection alive
  client.loop();

  // Check if it's time to send data and send it if necessary
  unsigned long currentMillis = millis();
  if(currentMillis - lastSendTime >= sendInterval) {
    sendBatteryData();                                        
    lastSendTime = currentMillis;                             // Update the last sent time
  }
}

// -----------------------------------------------------------------------------------------------------------------------
// Function Implementations

// Connects the ESP32 to a Wi-Fi network
void setupWiFi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);

  const unsigned long timeout = 10000;                                            // 10- second timeout
  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime) < timeout) {
    delay(500);
    Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected!");
    Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
    macAddress = WiFi.macAddress();                                         // Retrieve and store the MAC Address
    Serial.printf("MAC Address: %s\n", macAddress.c_str());                     
  } else {
    Serial.println("\nFailed to connect to Wi-Fi. Retrying...");
  }
}

// -----------------------------------------------------------------------------------------------------------------------

// Connects to the MQTT broker
void connectMQTT() {
  unsigned long lastAttemptTime = 0;              // Last connection attempt time
  const unsigned long reconnectInterval = 2000;   // Interval between reconnection attempts

  if(!client.connected() && (millis() - lastAttemptTime >= reconnectInterval)) {       
    Serial.println("Attempting to connect to MQTT broker...");
    if(client.connect("ESP32Publisher")) {        // MQTT client identifier
      Serial.println("Connected to MQTT broker!");
    } else {
      Serial.print("Connection failed, error code: ");
      Serial.print(client.state());
    }
    lastAttemptTime = millis();                   // Uptade the last attempt timestamp
  }
}

// -----------------------------------------------------------------------------------------------------------------------

// Simulates and sends battery data via MQTT
void sendBatteryData() {
  float voltage = random(3000,4200) / 1000.0;         // Simulates voltage between 3.0V and 4.2V
  float current = random(0,5000) / 1000.0;            // Simulates current between 0.0A and 5.0A
  float temperature = random(200,450) / 10.0;         // Simulates temperature beteween 20.0°C and 45.0°C

  // Format the data into a string
  char payload[100];
  snprintf(payload, sizeof(payload), "MAC: %s, Voltage: %.2fV, Current: %.3fA, Temperature: %.1f°C",
           macAddress.c_str(), voltage, current, temperature);

  // Publish the data to the MQTT topíc
  client.publish("battery/data", payload);
  
  // Display the sent data on the serial monitor
  Serial.print("\nData sent: ");
  Serial.println(payload);
}