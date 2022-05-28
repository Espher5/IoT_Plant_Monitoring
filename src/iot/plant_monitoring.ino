#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// OLED display 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

// Pump
#define ENABLE 14
#define DIRA 26
#define DIRB 27

// Light and air temperature/humidity sensors
#define LIGHT_PIN 32
#define DHT_PIN 


const char *SSID = "";
const char *PWD = "";
const char *MQTT_BROKER = "";
const int MQTT_PORT = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(WiFiClient);



void setup() {
	Serial.begin(115200);

}


void loop() {

}