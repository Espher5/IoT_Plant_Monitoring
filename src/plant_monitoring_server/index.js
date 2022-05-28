require('dotenv').config();
const express = require('express');
const cors = require('cors');
const mqttManager = require('./mqttReceiver');

const SERVER_PORT = process.env.SERVER_PORT || 3000;
const app = express();
app.use(cors());
mqttManager.initMQTT();


app.get('/', (req, res) => {
    res.send('Hello');
});

app.get('/api', (req, res) => {
    var messages = mqttManager.getMessages('/iot/water');
    res.json(messages);
}); 

app.listen(SERVER_PORT, (err) => {
    if(err) {
        throw err;
    }
    console.log('Server listening on port', SERVER_PORT);
});