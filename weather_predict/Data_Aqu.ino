void temphumid() {
  delay(2000);
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
}

void outPressure() {
  Serial.print("   Out Pressure = ");

  Serial.print(bme.readPressure());
  Serial.print(" Pa");

  outpr = bme.readPressure();
  delay(delayTime);
}
void inPressure() {

  Serial.print("   In Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.print(" Pa");
  inpr = bmp.readPressure();
}
void rain() {
  Serial.print("   Rain val = ");
  rainSt = analogRead(35);
  Serial.print(rainSt);
  if(rainSt>2000){
    rainval=1;
    }else{
      rainval=0;
      
      }

}

void getlight() {
  delay(ms);

  unsigned int data0, data1;
  if (light.getData(data0, data1))
  {

    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    good = light.getLux(gain, ms, data0, data1, lux);
    Serial.print(" lux: ");
    Serial.print(lux);
    lightval = lux;
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  }
  else
  {
    // getData() returned false because of an I2C error, inform the user.

    byte error = light.getError();
    printError(error);
  }


}
void printError(byte error)
// If there's an I2C error, this function will
// print out an explanation.
{
  Serial.print("I2C error: ");
  Serial.print(error, DEC);
  Serial.print(", ");

  switch (error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}
void sendData(int tem, int hum, int pre_out, int pre_in, int rain, int light) {
  String string_temperature =  String(tem, DEC);
  String string_humidity =  String(hum, DEC);
  String string_pre_out =  String(pre_out, DEC);
  String string_pre_in =  String(pre_in, DEC);
  String string_rain =  String(rain, DEC);
  String string_light =  String(light, DEC);
  String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity + "&pre_out=" + string_pre_out + "&pre_in=" + string_pre_in + "&rain=" + string_rain + "&light=" + string_light;
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    String payload;
    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload: " + payload);
    }
    http.end();
  }
}
