require('dotenv').config();
const nodemailer = require('nodemailer');


var emailTimer = 0;

const transporter = nodemailer.createTransport({
    service: 'hotmail',
    auth: {
        user: process.env.EMAIL_FROM_USER,
        pass: process.env.EMAIL_FROM_PWD
    }
});
transporter.verify().then(console.log).catch(console.err);


function sendEmail(message) {
    // Only send email if 60 seconds have passed from previous one
    var date =  new Date();
    var now = date.getTime();
    if(now - emailTimer < 60000) {
        console.log('Cannot send another email so soon!');
        return;
    }
    
    var mailOptions = {
        from: process.env.EMAIL_FROM_USER,
        to: process.env.EMAIL_TO_USER,
        subject: 'Warning from your Plant Monitoring system',
        text: message
    }
    
    /*
    transporter.sendMail(mailOptions, (error, info) => {
        if(error) {
            console.log(error);
            return;
        }
        console.log('Email sent: ' + info.response);
    });
    */
    emailTimer = now;
};


module.exports = {
    sendEmail
};