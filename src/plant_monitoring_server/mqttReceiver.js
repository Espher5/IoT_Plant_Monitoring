require('dotenv').config();
const mqtt = require('mqtt');

const host = process.env.MQTT_BROKER;
const port  = process.env.MQTT_PORT;
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`
const connectUrl = `mqtt://${host}:${port}`

const client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    username: 'emqx',
    password: 'public',
    reconnectPeriod: 1000
});


// Topic name - messages pair dictionary
var topics = {
    '/iot/water' : [],
    '/iot/moisture': [],
    '/iot/light': [],
    '/iot/temperature': [],
    '/iot/humidity': []
}


// Sets up the MQTT subscriptions
function initMQTT() {
    function subscribe(topic) {
        client.on('connect', () => {
            client.subscribe(topic, () => {
                console.log('Server subscribed to topic:', topic);
            });
        });
    }
    for(var key in topics) {
        subscribe(key);
    }

    client.on('message', (topic, payload) => {
        var message = payload.toString()
        console.log('Received message:', topic, message);

        topics[topic].push(message);
    });
}


// Returns the list of messages for a given topic
function getMessages(topic) {
    if(!Object.keys(topics).includes(topic)) {
        console.log('Server is not subscribed to topic');
        return;
    }

    return topics[topic];
}


module.exports = {
    initMQTT,
    getMessages
};