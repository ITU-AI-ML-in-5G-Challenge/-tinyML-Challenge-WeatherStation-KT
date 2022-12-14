#include "RandomForest.h"

Eloquent::ML::Port::RandomForest clf;
//////////////////////////////////////////////////////////////////


#include "DHT.h"
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;
unsigned long delayTime;
float outpr;
float inpr;

#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;


int rainSt;

#include <SparkFunTSL2561.h>
//#include <Wire.h>
SFE_TSL2561 light;
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds

double lightval;
int rainval = 0;
#include <WiFi.h>
#include <HTTPClient.h>
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

const char* ssid = "Dialog 4G";         // change SSID
const char* password = "kasun16855";    // change password
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbxppuGHtiUTO65OqXjgcSjoDNwAA89MAVtcBKKSYb6uGJwXTdjaY8Ipf_1m5oJqtvFiHQ";    // change Gscript ID
int count = 0;

///////////////////////////////////////////////////////////////


float sat_vap;
float actuatl_vap_pre;
float dry_prssure;
float Rd = 287.058;
float Rv = 461.495;




void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  dht.begin();
  bool status1;
  status1 = bme.begin(0x76);
  if (!status1) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  if (!bmp.begin(0x77)) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
  light.begin();


  unsigned char ID;
  if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID, HEX);
    Serial.println(", should be 0X5X");
  }
  // Most library commands will return true if communications was successful,
  // and false if there was a problem. You can ignore this returned value,
  // or check whether a command worked correctly and retrieve an error code:
  else
  {
    byte error = light.getError();
    printError(error);
  }

  gain = 0;

  unsigned char time = 2;

  Serial.println("Set timing...");
  light.setTiming(gain, time, ms);

  Serial.println("Powerup...");
  light.setPowerUp();
  // put your setup code here, to run once:

}

void loop() {

  temphumid() ;
  outPressure() ;
  inPressure();
  rain();
  int it = int(t);
  int ih = int(h);
  int ipo = int(outpr);
  int ipi = int(inpr);
  //sendData(it, ih, ipo, ipi, rainval, 0);
  //delay(60UL * 60UL * 1000UL);



  sat_vap = 6107.8 * pow(10, (7.5 * t / (t + 237.3)));
  actuatl_vap_pre = (sat_vap * h) / 100;
  dry_prssure = outpr - actuatl_vap_pre;

  float ro = (dry_prssure / (Rd * ( t + 273))) + ( actuatl_vap_pre / (Rv * (t + 273)));
  float wind_speed = sqrt(2 * (inpr - outpr) / ro);
  /*
    float temp= random(25,30)+0.1;
    float humid= random(95,99)+0.1;
    float rain= random(0,1);
    float wind= random(20,35)+0.1;
    float pre= random(1000,1009)+0.1;
  */
  float new_weather[5] = {t, h , rainval , wind_speed , outpr};
  Serial.print("Chance of rain (%) : ");
  Serial.println(clf.predict(new_weather));
  int winds = int(wind_speed);

  //26.2,91.27 ,1.0 , 6.2 ,1011.6 for testing 
  sendData(it, ih, ipo, ipi, rainval, wind_speed);// insted of lux I send wind speed
  delay(60UL * 60UL * 1000UL);
  // delay(1000);

  // put your main code here, to run repeatedly:

}
