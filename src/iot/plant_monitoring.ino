#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include "arduino_secrets.h"


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
#define DHT_PIN 33
#define DHT_TYPE DHT11

// Button


const char *SSID = "";
const char *PWD = "";
const char *MQTT_BROKER = "";
const int MQTT_PORT = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(WiFiClient);

DHT dht(DHT_PIN, DHT_TYPE)

long last_time = 0;
char data[100];


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
    mqttClient.setServer(mqttServer, mqttPort);
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
            // subscribe to topic
            mqttClient.subscribe("/swa/commands");
        }    
    }
}


void setup() {
	Serial.begin(115200);
    connectToWiFi();
    setupMQTT();
    pinMode(WATER_POWER_PIN, OUTPUT);
    
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
    digitalWrite(WATER_POWER_PIN, LOW);

    // Moisture level sensor
    // ~ -300 (dry) to  -100 (very wet)
    moistureLevel = ( 100.00 - ( (analogRead(SOIL_MOISTURE_PIN) / 1023.00) * 100.00 ) );
    moisturePercentage = map(moistureLevel, -300, -100, 0, 100);

    // Light level sensor
    lightLevel = analogRead(LIGHT_PIN);

    // Air temperature and humidity sensor
    airTemperature = dht.readTemperature();
    airHumidity = dht.readHumidity();


    // Send MQTT messages
    if (!mqttClient.connected()) {
        reconnect();
    }    
    mqttClient.loop();

    long now = millis();
    if(now - last_time > 3000) {
        int lightLevel = analogRead(LIGHT_SENSOR_PIN);
        sprintf(data, "%d", lightLevel);
        Serial.println(data);
        mqttClient.publish("/swa/lightLevel", data);

        last_time = now;
    }  
}