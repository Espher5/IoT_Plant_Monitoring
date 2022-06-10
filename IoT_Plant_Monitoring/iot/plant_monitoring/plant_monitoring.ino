#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>


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


const char *SSID = "IotLab";
const char *PWD = "vmum7999";
const char *MQTT_BROKER = "192.168.112.165";
const int MQTT_PORT = 1883;

// Variables to store sensor measurements
int waterLevel, waterLevelPercentage;
int moistureLevel, moistureLevelPercentage;
int lightLevel;
int airTemperature;
int airHumidity;

char data[100];
long pumpTimer = -60000;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 

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
    Serial.println("Connected.\n");
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
            // subscribe to topic
            mqttClient.subscribe("/iot/commands");
        }
        
    }
}

// Initialize OLED display settings
void setupDisplay() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("SSD1306 allocation failed");
        for(;;); // Don't proceed, loop forever
    }
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setTextWrap(false);
}


// Updates the status information on the OLED display
void updateDisplay() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("IoT Plant Monitoring");

    display.setCursor(0, 10);
    display.print("Soil moisture: ");
    display.print(moistureLevelPercentage);

    display.setCursor(0, 20);
    display.print("Water level: ");
    display.print(waterLevelPercentage);

    display.setCursor(0, 30);
    display.print("Light level: ");
    display.print(lightLevel);

    display.setCursor(0, 40);
    display.print("Air temperature: ");
    display.print(airTemperature);

    display.setCursor(0, 50);
    display.print("Air humidity: ");
    display.print(airHumidity);
    
    display.display();
}


void setup() {
    Serial.begin(115200);
    connectToWiFi();
    setupMQTT();
    pinMode(WATER_POWER_PIN, OUTPUT); 
    dht.begin();
    setupDisplay();
    delay(1000); 
}


void loop() {
    // Collect the sensor measurements
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
    
    // If the moisture level is < 20% the pump is activated
    long now = millis();
    if(moistureLevelPercentage < 20) {
        if(now - pumpTimer < 60000) {
            Serial.println("Pump was activated a short time ago");  
        } else {    
            Serial.println("Activating pump...");
            digitalWrite(DIRA, HIGH);
            digitalWrite(DIRB, LOW);
            analogWrite(ENABLE, 128);
            delay(500);
            
            Serial.println("Stopping pump...");      
            digitalWrite(DIRA, LOW);
            digitalWrite(DIRB, LOW);
            digitalWrite(ENABLE, LOW);
            delay(500);
        }
    }
    Serial.println();
    Serial.println();

    updateDisplay();
    
    if (!mqttClient.connected())
        reconnect();
    mqttClient.loop();
  
    sprintf(data, "%d", waterLevelPercentage);
    mqttClient.publish("/iot/water", data);
    sprintf(data, "%d", moistureLevelPercentage);
    mqttClient.publish("/iot/moisture", data);
    sprintf(data, "%d", lightLevel);
    mqttClient.publish("/iot/light", data);
    sprintf(data, "%d", airTemperature);
    mqttClient.publish("/iot/temperature", data);
    sprintf(data, "%d", airHumidity);
    mqttClient.publish("/iot/humidity", data);

    delay(10000);
}