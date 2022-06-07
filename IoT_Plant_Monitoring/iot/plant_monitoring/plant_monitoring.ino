#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
//#include "arduino_secrets.h"


// OLED display 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

// Pump
#define ENABLE 14
#define DIRA 26
#define DIRB 27

// Water level sensor
#define WATER_POWER_PIN  16
#define WATER_SIGNAL_PIN 34

// Soil Moisture sensor
#define SOIL_MOISTURE_PIN A0 // SP pin on ESP32

// Light and air temperature/humidity sensors
#define LIGHT_PIN 32
#define DHT_PIN 25
#define DHT_TYPE DHT11

// Button



const char *SSID = "IotLab";
const char *PWD = "vmum7999";
const char *MQTT_BROKER = "192.168.193.165";
const int MQTT_PORT = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 

long last_time = 0;
char data[100];

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHT_PIN, DHT_TYPE);


// Connect to WiFi and setup MQTT connection to the broker
void connectToWiFi() {
    Serial.print("Connecting to ");
 
    WiFi.begin(SSID, PWD);
    Serial.println(SSID);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("Connected.");
}


// Callback function for receiving messages
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Callback - ");
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
}


void setupMQTT() {
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(callback);
}


void reconnect() {
    Serial.println("Connecting to MQTT Broker...");
    while (!mqttClient.connected()) {
        Serial.println("Reconnecting to MQTT Broker..");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
      
        if (mqttClient.connect(clientId.c_str())) {
            Serial.println("Connected.");
            // Subscribe to return topic for debug purposes
            mqttClient.subscribe("/iot/commands");
        }      
  }
}


void setup() {
    Serial.begin(115200);
    connectToWiFi();
    setupMQTT();
    pinMode(WATER_POWER_PIN, OUTPUT);  
    dht.begin();
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("SSD1306 allocation failed");
        for(;;); // Don't proceed, loop forever
    }
    display.display();
    display.clearDisplay();
    delay(1000);
}


void loop() {
    // Collect the sensor measurements
    int waterLevel, waterLevelPercentage;
    int moistureLevel, moistureLevelPercentage;
    int lightLevel;
    float airTemperature;
    float airHumidity;

    // Water level sensor 
    digitalWrite(WATER_POWER_PIN, HIGH);
    delay(10); 
    waterLevel = analogRead(WATER_SIGNAL_PIN);
    waterLevelPercentage = map(waterLevel, 0, 4096, 0, 100);
    Serial.print("Water level percentage: ");
    Serial.println(waterLevelPercentage);
    digitalWrite(WATER_POWER_PIN, LOW);

    // Moisture level sensor
    // ~ -300 (dry) to  -100 (very wet)
    moistureLevel = ( 100.00 - ( (analogRead(SOIL_MOISTURE_PIN) / 1023.00) * 100.00 ) );
    moistureLevelPercentage = map(moistureLevel, -300, -100, 0, 100);
    Serial.print("Moisture percentage: ");
    Serial.println(moistureLevelPercentage);

    // If the moisture level is < 20% the pump is activate
    if(moistureLevelPercentage < 20) {

    }

    // Light level sensor
    lightLevel = analogRead(LIGHT_PIN);
    Serial.print("Light level: ");
    Serial.println(lightLevel);

    // Air temperature and humidity sensor
    airTemperature = dht.readTemperature();
    airHumidity = dht.readHumidity();
    Serial.print("Air temperature: ");
    Serial.println(airTemperature);
    Serial.print("Air humidity: ");
    Serial.println(airHumidity);
    
    Serial.println();
    Serial.println();
    
    if (!mqttClient.connected())
        reconnect();
    mqttClient.loop();
  
    long now = millis();
    if(now - last_time > 1000) {
        sprintf(data, "%d", waterLevelPercentage);
        mqttClient.publish("/iot/water", data);

        sprintf(data, "%d", moistureLevelPercentage);
        mqttClient.publish("/iot/moisture", data);

        sprintf(data, "%d", lightLevel);
        mqttClient.publish("/iot/light", data);
        last_time = now;
    }
    delay(10000);
}