#include <ESP8266WiFi.h>
#include <Arduino.h>
// #include <DNSServer.h>
// #include <ESPAsyncWebServer.h>
// #include <ArduinoJson.h>
#include <OneButton.h>
// #include <Schedule.h>
// #include <WiFiUdp.h>
// #include <ArduinoOTA.h>
// #include <ESP8266mDNS.h>

int WATER_LEVEL = A0;
int BUTTON1 = D8;
int BUTTON2 = D6;


// OneButton btn = OneButton(
//     BUTTON1, // Input pin for the button
//     true,  // Button is active LOW
//     true   // Enable internal pull-up resistor
// );
// unsigned long pressStartTime;
// IRAM_ATTR void checkTicks()
// {
//   // include all buttons here to be checked
//   btn.tick(); // just call tick() to check the state.
// }

// void singleClick()
// {
//   //if error state - try recconecting to ssid <- (Might be able to just esp restart?)
//   Serial.println("singleClick() detected.");

//   // lightControl(0xFF0000);
// } // singleClick

// // this function will be called when the button was held down for 1 second or more.
// void pressStart()
// {
//   Serial.println("pressStart()");
//   pressStartTime = millis() - 1000; // as set in setPressTicks()

// } // pressStart()

// // this function will be called when the button was released after a long hold.
// void pressStop()
// {
//   Serial.print("pressStop(");
//   Serial.print(millis() - pressStartTime);
//   Serial.println(") detected.");


// } // pressStop()


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(200);
  Serial.print("I'm Awake");
  //   attachInterrupt(digitalPinToInterrupt(BUTTON1), checkTicks, CHANGE);
  // btn.attachClick(singleClick);
  // // btn.attachDoubleClick(doubleClick);
  // // btn.attachMultiClick(multiClick);
  // btn.setPressTicks(1000); // that is the time when LongPressStart is called
  // btn.attachLongPressStart(pressStart);
  // btn.attachLongPressStop(pressStop);

}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(WATER_LEVEL);
// Serial.print("Analog value: ");
// Serial.print(analogValue);
// Serial.println("");
int buttonVal = digitalRead(BUTTON1);
Serial.print("Button1 value: ");
Serial.print(buttonVal);
Serial.println("");
int buttonVal2 = digitalRead(BUTTON2);
Serial.print("Button2 value: ");
Serial.print(buttonVal2);
Serial.println("");
delay(200);
}
