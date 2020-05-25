const functions = require("firebase-functions");

const admin = require("firebase-admin");
admin.initializeApp();

const app = require("express")();

app.get("/Weather_Data", async (req, res) => {
  const arrays = [];
  var Weather_Data = await admin
    .database()
    .ref("/Weather_Data")
    .once("value")
    .then((gotData) => {
      
      gotData.forEach((snapshiot) => {
       // const temp = snapshiot.val().Temperature;
       
        arrays.push( temp=snapshiot.val().Temperature);
      });
      return arrays;
    })
    .then((rres) => res.send(rres))
    .catch(error => res.send(error));

});

app.get("/getDailyWeatherData", async (req, res) => {
  await admin
    .database()
    .ref("/Weather_Data")
    .orderByChild("Time")
    .startAt("15:51:33")
    .endAt("15:52:44")
    .once("value", gotData);

  function gotData(data) {
    var dataweather = data.val();
    let array1 = [];
    array1.push(Object.values(dataweather));

    console.log(array1);
    res.send(array1);
  }
  function errData(err) {
    console.log(err);
  }
});

exports.api = functions.region("europe-west1").https.onRequest(app);
