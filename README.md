# Iot_plant_monitoring
An IoT system to monitor various parameters of a plant

An ESP32 development board is used to collect data from various IoT sensors monitoring a plant in a controlled environment.
These measurements are published as MQTT messages after some initial processing. An MQTT broker implemented on a Raspberry Pi listens on various topics for the messages published.
Finally, a web server built with NodeJS and Express subscribes to these topics and sends the collected data to a client application made with Vue.js, which displays these measurements to the user with real-time charts.


The architecture of the system is the following