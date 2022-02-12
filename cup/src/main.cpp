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
int BUTTON1 = D1;
int BUTTON2 = D2;


OneButton btn = OneButton(
    BUTTON1, // Input pin for the button
    true,  // Button is active LOW
    true   // Enable internal pull-up resistor
);
unsigned long pressStartTime;
IRAM_ATTR void checkTicks()
{
  // include all buttons here to be checked
  btn.tick(); // just call tick() to check the state.
}



void singleClick()
{
  //if error state - try recconecting to ssid <- (Might be able to just esp restart?)
  Serial.println("singleClick() detected.");

  // lightControl(0xFF0000);
} // singleClick

// this function will be called when the button was held down for 1 second or more.
void pressStart()
{
  Serial.println("pressStart()");
  pressStartTime = millis() - 1000; // as set in setPressTicks()

} // pressStart()

// this function will be called when the button was released after a long hold.
void pressStop()
{
  Serial.print("pressStop(");
  Serial.print(millis() - pressStartTime);
  Serial.println(") detected.");


} // pressStop()

OneButton btn2 = OneButton(
    BUTTON2, // Input pin for the button
    true,  // Button is active LOW
    true   // Enable internal pull-up resistor
);

unsigned long pressStartTime2;
IRAM_ATTR void checkTicks2()
{
  // include all buttons here to be checked
  btn2.tick(); // just call tick() to check the state.
}


void singleClick2()
{
  //if error state - try recconecting to ssid <- (Might be able to just esp restart?)
  Serial.println("singleClick2() detected.");

  // lightControl(0xFF0000);
} // singleClick

// this function will be called when the button was held down for 1 second or more.
void pressStart2()
{
  Serial.println("pressStart2()");
  pressStartTime2 = millis() - 1000; // as set in setPressTicks()

} // pressStart()

// this function will be called when the button was released after a long hold.
void pressStop2()
{
  Serial.print("pressStop(");
  Serial.print(millis() - pressStartTime2);
  Serial.println(")2 detected.");


} // pressStop()


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(200);
  Serial.print("I'm Awake");
    attachInterrupt(digitalPinToInterrupt(BUTTON1), checkTicks, CHANGE);
  btn.attachClick(singleClick);
  // btn.attachDoubleClick(doubleClick);
  // btn.attachMultiClick(multiClick);
  btn.setPressTicks(1000); // that is the time when LongPressStart is called
  btn.attachLongPressStart(pressStart);
  btn.attachLongPressStop(pressStop);

      attachInterrupt(digitalPinToInterrupt(BUTTON2), checkTicks2, CHANGE);
  btn2.attachClick(singleClick2);
  // btn.attachDoubleClick(doubleClick);
  // btn.attachMultiClick(multiClick);
  btn2.setPressTicks(1000); // that is the time when LongPressStart is called
  btn2.attachLongPressStart(pressStart2);
  btn2.attachLongPressStop(pressStop2);

}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(WATER_LEVEL);
// Serial.print("Analog value: ");
// Serial.print(analogValue);
// Serial.println("");
// int buttonVal = digitalRead(BUTTON1);
// Serial.print("Button1 value: ");
// Serial.print(buttonVal);
// int buttonVal2 = digitalRead(BUTTON2);
// //flip the value of the button
// Serial.print(" Button2 value: ");
// Serial.print(buttonVal2);
// Serial.println("");
delay(200);
}
