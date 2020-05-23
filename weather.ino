#include "WiFi.h"
#include <FirebaseESP32.h>
#define FIREBASE_HOST "weatherstation-f0279.firebaseio.com"
#define FIREBASE_AUTH "4rG1DFQUB3z2g3LgBkBnkr9KMICXLcNI9jJs2rXu"

#include <NTPClient.h>
#include <WiFiUdp.h>
FirebaseData firebaseData;

const char* ssid = "TP-LINK_POCKET_3020_A9F252";
const char* password =  "Liam2018";
String path = "/Weather_Data";
int count = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;
String dayStamp;
String timeStamp;
unsigned long previousMillis_one = 0; // store the last time updated
unsigned long previousMillis_two = 0; // store the last time updated

long one_minute_interval = 1 * 60 * 1000UL; // interval desired (millisenconds)
long five_minutes_interval = 5 * 60 * 1000UL; // interval desired (millisenconds)
String exact_minute;


char                 databuffer[35];
double               temp;

void getBuffer()                                                                    //Get weather status data
{
  int index;
  for (index = 0; index < 35; index ++)
  {
    if (Serial.available())
    {
      databuffer[index] = Serial.read();
      if (databuffer[0] != 'c')
      {
        index = -1;
      }
    }
    else
    {
      index --;
    }
  }
}

int transCharToInt(char *_buffer, int _start, int _stop)                             //char to int）
{
  int _index;
  int result = 0;
  int num = _stop - _start + 1;
  int _temp[num];
  for (_index = _start; _index <= _stop; _index ++)
  {
    _temp[_index - _start] = _buffer[_index] - '0';
    result = 10 * result + _temp[_index - _start];
  }
  return result;
}

int WindDirection()                                                                  //Wind Direction
{
  return transCharToInt(databuffer, 1, 3);
}

float WindSpeedAverage()                                                             //air Speed (1 minute)
{
  temp = 0.44704 * transCharToInt(databuffer, 5, 7);
  return temp;
}

float WindSpeedMax()                                                                 //Max air speed (5 minutes)
{
  temp = 0.44704 * transCharToInt(databuffer, 9, 11);
  return temp;
}

float Temperature()                                                                  //Temperature ("C")
{
  temp = (transCharToInt(databuffer, 13, 15) - 32.00) * 5.00 / 9.00;
  return temp;
}

float RainfallOneHour()                                                              //Rainfall (1 hour)
{
  temp = transCharToInt(databuffer, 17, 19) * 25.40 * 0.01;
  return temp;
}

float RainfallOneDay()                                                               //Rainfall (24 hours)
{
  temp = transCharToInt(databuffer, 21, 23) * 25.40 * 0.01;
  return temp;
}

int Humidity()                                                                       //Humidity
{
  return transCharToInt(databuffer, 25, 26);
}

float BarPressure()                                                                  //Barometric Pressure
{
  temp = transCharToInt(databuffer, 28, 32);
  return temp / 10.00;
}


void setup() {

  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  timeClient.begin();

  timeClient.setTimeOffset(10800);
  if (!Firebase.pathExist(firebaseData, path ))
  {



  }



}




void loop() {

  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);
  int splitT = formattedDate.indexOf("T");
  //exact date
  dayStamp = formattedDate.substring(0, splitT);
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
  Serial.println(timeStamp);

  delay(60000);
  getBuffer();


  if (!Firebase.pathExist(firebaseData, path + "/" + formattedDate ))
  {


    String path2 = path + "/" + formattedDate;
    Firebase.set(firebaseData, path2 +  "/Date", dayStamp);
    Firebase.set(firebaseData, path2 +  "/Time", timeStamp);
    Firebase.set(firebaseData, path2 +  "/Wind_direction", WindDirection());
    Firebase.set(firebaseData, path2 + "/A_Windsepeed", WindSpeedAverage());
    Firebase.set(firebaseData, path2 + "/M_Windspeed", WindSpeedMax());
    Firebase.set(firebaseData, path2 + "/Rainfall_1hr", RainfallOneHour());
    Firebase.set(firebaseData, path2 + "/Rainfall_24hr",  RainfallOneDay());
    Firebase.set(firebaseData, path2 + "/Temperature", Temperature());
    // delay(2000);
    Firebase.set(firebaseData, path2 + "/Humidity", Humidity());
    // delay(2000);
    Firebase.set(firebaseData, path2 + "/Barometric_Pressure", BarPressure());


  }




  //Begin!
  delay(2000);
  Serial.print("Wind Direction: ");

  Serial.print(WindDirection());
  Serial.println("  ");
  delay(2000);
  Serial.print("Average Wind Speed (One Minute): ");
  Serial.print(WindSpeedAverage());
  Serial.println("m/s  ");
  delay(500);
  Serial.print("Max Wind Speed (Five Minutes): ");
  Serial.print(WindSpeedMax());
  Serial.println("m/s");
  Serial.print("Rain Fall (One Hour): ");
  Serial.print(RainfallOneHour());
  Serial.println("mm  ");
  Serial.print("Rain Fall (24 Hour): ");
  Serial.print(RainfallOneDay());
  Serial.println("mm");
  Serial.print("Temperature: ");
  Serial.print(Temperature());
  Serial.println("C  ");
  Serial.print("Humidity: ");
  Serial.print(Humidity());
  Serial.println("%  ");
  Serial.print("Barometric Pressure: ");
  Serial.print(BarPressure());
  Serial.println("hPa");
  Serial.println("");
  Serial.println("");
}
