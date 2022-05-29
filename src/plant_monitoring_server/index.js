require('dotenv').config();
const express = require('express');
const cors = require('cors');
const mqttManager = require('./mqttReceiver');

const SERVER_PORT = process.env.SERVER_PORT || 3000;
const app = express();

app.use(cors());

app.use((req, res, next) => {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With, content-type, Authorization');
    res.setHeader('Access-Controll-Allow-Credentials', true);
    next();
});


app.get('/', (req, res) => {
    res.status(200).json({ info: 'Plant monitoring server built with NodeJS and Express'})
});

app.get('/api', (req, res) => {
    var topic = req.query.topic;
    if(topic === null) {
        return;
    }

    console.log('Received request for messages in topic', topic);
    var messages = mqttManager.getMessages('/iot/water');
    res.json(messages);
}); 

app.listen(SERVER_PORT, (err) => {
    if(err) {
        throw err;
    }
    console.log('Server listening on port', SERVER_PORT);
});

mqttManager.initMQTT();