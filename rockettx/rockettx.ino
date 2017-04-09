#include <Wire.h>

//PINS
//ALTSOFSERIAL RX IS PIN 8
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define USE_SERIAL_MONITOR true

//From GPS: 3 rx ,4 tx
SoftwareSerial ss(4, 3);
TinyGPS gps;

//From XBEE:  
SoftwareSerial xBee(8,9);

void shiftWrite(String message) {
  xBee.begin(9600);
  
  for (int i = 0; i < message.length(); i++) {
    xBee.write(message[i]);
  }

  xBee.end();
}

void setup() {
  if (USE_SERIAL_MONITOR) {
    Serial.begin(115200);
  }
  
  ss.begin(9600);
  
}

void loop() {
  //initialize variable to store data to
  String dataString;
  dataString += "Time(sec): " + String( (millis()/1000)) ;

  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

  bool newData = false;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

//  if (newData)
//  {
    gps.f_get_position(&flat, &flon, &age);
    dataString += " | LAT: " + String(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);

    dataString += " | LON: " + String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);

    dataString += " | SAT: " + String(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    dataString += " | Speed(kmph): " + String(String(gps.f_speed_kmph()));
    dataString += " | Altitude(m): " + String( gps.f_altitude()) + "\n";

  ss.end();

//  byte dataBuf[100];
//
//  for(int i=0; i<100; i++){
//    dataBuf[i]=dataString[i];
//  }
//  
//  strcpy((char *)dataBuf, (char *)dataBuf);
//  delay(10);
  if (USE_SERIAL_MONITOR) {
    Serial.println(dataString);
  }
  
  shiftWrite(dataString);
  
//  }
//  Serial.println("Message Sent");
  
  delay(1000);
}
