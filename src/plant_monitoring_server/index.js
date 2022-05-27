require('dotenv').config();
const express = require('express');
const cors = require('cors');
const mqttManager = require('./mqttReceiver');

const SERVER_PORT = process.env.SERVER_PORT || 3000;
const app = express();
app.use(cors());
mqttManager.initMQTT();


app.get('/', (req, res) => {
    console.log(mqttManager.getMessages('/iot/water'));
    res.send('Hello');
});

app.listen(SERVER_PORT, (err) => {
    if(err) {
        throw err;
    }
    console.log('Server listening on port', SERVER_PORT);
});