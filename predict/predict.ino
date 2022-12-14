#include "RandomForest.h"

Eloquent::ML::Port::RandomForest clf;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
float t=26.2;
float h=91.27;
float rainval=1.0 ;
float wind_speed=6.2;
float outpr=1011.6;

  float new_weather[5] = {t, h , rainval , wind_speed , outpr};
  Serial.print("Chance of rain (%) : ");
  Serial.println(clf.predict(new_weather));
  delay(5000);
  // put your main code here, to run repeatedly:

}
