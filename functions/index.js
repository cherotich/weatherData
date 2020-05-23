const functions = require('firebase-functions');

const admin = require('firebase-admin');
admin.initializeApp();

exports.getWeatherData = functions.https.onRequest(async (req,res)=>{
 
    var Weather_Data=  await admin.database().ref('/Weather_Data').once('value');
    res.send(Weather_Data)
});

exports.getDailyWeatherData = functions.https.onRequest(async (req,res) =>{
    var daily = await admin.database().ref('/Weather_Data').orderByChild('Time').startAt('15:51:33').endAt('15:56:10').once('value');
    res.send(daily)
});