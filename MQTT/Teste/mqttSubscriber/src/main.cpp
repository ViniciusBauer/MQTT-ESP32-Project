// -----------------------------------------------------------------------------------------------------------------------
// Program Module: MQTT Subscriber Test
// -----------------------------------------------------------------------------------------------------------------------
//        
// -----------------------------------------------------------------------------------------------------------------------
// Description:
// This code demonstrates how to connect an ESP32 to a Wi-Fi network, subscribe to an MQTT broker,
// and process messages received on the subscribed topic. The received messages are expected to
// contain data like MAC address, voltage, current, and temperature. The subscriber handles the 
// connection to the broker, receiving data, and parsing the messages accordingly.
// -----------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------
// Libraries

#include <Arduino.h>
#include <WiFi.h>                            // Library for Wi-Fi connection
#include <PubSubClient.h>                    // Library for MQTT communication
// #include "freertos/FreeRTOS.h"            
// #include "freertos/task.h"                  
// #include "freertos/queue.h"                 

// -----------------------------------------------------------------------------------------------------------------------
// Hardware mapping

// -----------------------------------------------------------------------------------------------------------------------
// Configurations

// Wi-Fi configurations     
const char* ssid = "ALHN-0AA3";                            // Wi-Fi network name (SSID)
const char* password = "ZFbS39S@Zi";                    // Wi-Fi password

// MQTT configurations
const char* mqttServer = "test.mosquitto.org";        // MQTT broker address
const int mqttPort = 1883;                            // MQTT broker port
const char* mqttUser = "mqqtUser";                    // (Optional) MQTT username
const char* mqttPassword = "mqttPassword";            // (Optional) MQTT password

// -----------------------------------------------------------------------------------------------------------------------
// Global Objects and Variables

WiFiClient espClient;                                 // Object to manage Wi-Fi connection
PubSubClient client(espClient);                       // Object to manage MQTT connection

// -----------------------------------------------------------------------------------------------------------------------
// Protótipo das Funções

void setupWiFi();                                                   // Initialize Wi-Fi connection
void connectMQTT();                                                 // Connect to the MQTT broker
void callback(char* topic, byte* payload, unsigned int lenght);     // Function to handle received messages

// -----------------------------------------------------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);                               // Initialize serial communication
    setupWiFi();                                        // Set up Wi-Fi connection
    client.setServer(mqttServer, mqttPort);             // Configure the MQTT broker
    client.setCallback(callback);                       // Set the callback function for receiving messages
}

// -----------------------------------------------------------------------------------------------------------------------

void loop() {
  // Checl if the MQTT client is connected
  if (!client.connected()){
    connectMQTT();                                        // Attempt to connect to MQTT broker
  }

  // Keep the MQTT connection alive
  client.loop();      
}

// -----------------------------------------------------------------------------------------------------------------------
// Desenvolvimento das Funções

void setupWiFi() {
  Serial.print("Connecting to Wi-Fi..."); 
  WiFi.begin(ssid, password);                         // Attempt to connect to Wi-Fi
  
  const unsigned long timeout = 10000;                // Timeout duration for Wi-Fi connection
  unsigned long startAttemptTime = millis();           // Track connection attempt time

  // Wait for connection or timeout
  while(WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime < timeout)) {
    delay(500);                                     
    Serial.print(".");
  }

  // Check if connected or if there was a timeout
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected!");
    Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\nFailed to connect to Wi-Fi. Retrying...");
  }
}

// -----------------------------------------------------------------------------------------------------------------------

void connectMQTT() {
  unsigned long lastAttemptTime = 0;              // Last connection attempt time
  const unsigned long reconnectInterval = 2000;   // Interval between reconnection attempts

  // Verifica se o intervalo entre as tentativas já foi atingido
  if(!client.connected() && (millis() - lastAttemptTime >= reconnectInterval)) {
    Serial.println("Attempting to connect to MQTT broker...");
    if(client.connect("ESP32Subscriber")) {
      Serial.println("Connected to MQTT broker");

      // Subscribe to topic
      client.subscribe("battery/data");
    } else {
      Serial.print("Failed to connect, error code: ");
      Serial.println(client.state());
    }

    // Update the last connection attempt time
    lastAttemptTime = millis();
  }
}

// -----------------------------------------------------------------------------------------------------------------------

void callback(char* topic, byte* payload, unsigned int lenght) {
  char macAddress[18];                                                // Array to store MAC Address
  float voltage, current, temperature;                                // Variables to sorte received message values

  // Convert payload to string
  String message = "";
  for(int i = 0; i < lenght; i++) {
    message += (char)payload[i];
  }

  // Expects payload in the format: "MAC:XX:XX:XX:XX:XX:XX, Voltage: X.XX, Current: X.XXX, Temperature: XX.X"
  if(sscanf(message.c_str(), "MAC: %17s, Voltage: %fV, Current: %fA, Temperature: %f°C", macAddress, &voltage, &current, &temperature) == 4) {
    // Print the extracred values
    Serial.printf("\nData received from MAC: %s\n", macAddress);
    Serial.printf("Voltage: %.2f V, Current: %.3f A, Temperature: %.1f °C\n", voltage, current, temperature);
  } else {
    // If the message format is incorrect
    Serial.println("Error processing the message!");
  }
}
