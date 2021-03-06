require('dotenv').config();
const mqtt = require('mqtt');
const emailHandler = require('./emailHandler');

const host = process.env.MQTT_BROKER;
const port  = process.env.MQTT_PORT;
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`;
const connectUrl = `mqtt://${host}:${port}`;


const client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    username: 'IotLab',
    password: 'public',
    reconnectPeriod: 1000
});


var topics = {
    '/iot/water' : {
        messages: [],
        minValue: 0,
        maxValue: 100
    },
    '/iot/moisture': {
        messages: [],
        minValue: 0,
        maxValue: 100
    },
    '/iot/light': {
        messages: [],
        minValue: 0,
        maxValue: 4096
    },
    '/iot/temperature': {
        messages: [],
        minValue: -40,
        maxValue: 125
    },
    '/iot/humidity': {
        messages: [],
        minValue: 0,
        maxValue: 100
    }
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
        console.log('Received message "', message, '" from topic', topic);     
        var controlString = processMessage(topic, message);
        if(controlString) {
            console.log(controlString);
            // Send email to the user in case of warnings
            emailHandler.sendEmail(controlString);          
        }
        topics[topic].messages.push(message);
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


function processMessage(topic, message) {
    var topicData = topics[topic];
    console.log(topicData);
    if(message < topicData.minValue || message > topicData.maxValue) {
        return 'Data out of allowed range';
    }
    if(!typeof(message) === 'number') {
        return 'Invalid data type for message';
    }

    switch(topic) {
        case '/iot/water':
            if(message < 15) {
                return 'Warning, water supply is running low, please refill as soon as you can.';
            }
            break;
        
        case '/iot/moisture':
            if(message < 30) {
                return 'Warning, low moisture detected in soil. Please check water supplies and pump status.';
            }
            break;
        case '/iot/light':
            if(message > 3000) {
                return 'Warning, high light levels detected near the plant!';
            }
            break;
        case '/iot/temperature':
            if(message > 25) {
                return 'Warning, high temperatures detected near the plant!';
            }
            break;
        case '/iot/humidity':
            if(message > 40) {
                return 'Warning, high humidity levels detected near the plant!';
            }
            break;
        default:
            return '';
    }
}

module.exports = {
    initMQTT,
    getMessages
};