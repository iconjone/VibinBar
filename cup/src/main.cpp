#include <ESP8266WiFi.h>
#include <Arduino.h>
// #include <DNSServer.h>
// #include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <OneButton.h>
#include <ArduinoWebsockets.h>
#include <Schedule.h>
// #include <WiFiUdp.h>
// #include <ArduinoOTA.h>
// #include <ESP8266mDNS.h>

int WATER_LEVEL = A0;
int BUTTON1 = D1;
int BUTTON2 = D2;

int RED_LED = D8;   
int GREEN_LED = D7; 
int BLUE_LED = D6;  
const char* ssid     = "VibinBar";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "vibinbar";         // The password of the Wi-Fi network
int led = 0;
String cup = "B";

std::function<void(void)> lightControl(int hex)
{
  analogWrite(RED_LED, (hex >> 16) * 4);
  analogWrite(GREEN_LED, ((hex >> 8) & 0xFF) * 4);
  analogWrite(BLUE_LED, (hex & 0xFF) * 4);
  return 0;
}

std::function<void(void)> blinkLight(int hex)
{
  lightControl(hex);
  delay(500);
  lightControl(0x0);
  delay(500);
  lightControl(hex);
  delay(500);
  lightControl(0x0);
  //delay(500);
  return 0;
}

const char* websockets_server = "ws://192.168.137.30:80/ws"; //server adress and port

using namespace websockets;

void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
    int len = message.data().length();
            StaticJsonDocument<96> doc;

    DeserializationError error = deserializeJson(doc, message.c_str());
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }
    const char* TYPE = doc["TYPE"]; 
    //check if TYPE is equal to "cupMessage"
    if (strcmp(TYPE, "cupMessage") == 0) {
        const char* CUP = doc["CUP"];
        //check if CUP is equal to cup
        if (strcmp(CUP, cup.c_str()) == 0) {
           Serial.println("Light up this cup");
            blinkLight(0x00FF00);
        }
    }
        else if (strcmp(TYPE, "percentage") == 0) {
        const char* CUP = doc["CUP"];
        //check if CUP is equal to cup
        if (strcmp(CUP, cup.c_str()) == 0) {
          int percentage = doc["PERCENTAGE"];
           Serial.println("Light up this cup");
           int RED = (300 * (100 - percentage)) / 100 ;
            int GREEN = (percentage * 255) / 100;
            int BLUE = 0;
            int value = (RED << 16) | (GREEN << 8) | BLUE;
            if(value != led){
              led = value;
            lightControl(value);
            }
        }
    }

}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

WebsocketsClient client;

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



bool singleClickBool = false;
bool doubleClickBool = false;
void singleClick()
{
  //if error state - try recconecting to ssid <- (Might be able to just esp restart?)
  Serial.println("singleClick() detected.");

  // lightControl(0xFF0000);
  // schedule_function(lightControl(0x0000FF));
  singleClickBool = true;



} // singleClick

void doubleClick()
{
  //if error state - try recconecting to ssid <- (Might be able to just esp restart?)
  Serial.println("doubleClick() detected.");
  // schedule_function(lightControl(0xFF00FF));
    doubleClickBool = true;
  


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



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(200);
  Serial.print("I'm Awake");
    attachInterrupt(digitalPinToInterrupt(BUTTON1), checkTicks, CHANGE);
      btn.setPressTicks(1000); // that is the time when LongPressStart is called
  btn.attachLongPressStart(pressStart);
  btn.attachLongPressStop(pressStop);
  btn.attachClick(singleClick);
  btn.attachDoubleClick(doubleClick);
  // btn.attachMultiClick(multiClick);


 

WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());   

client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    
    // Connect to server
    client.connect(websockets_server);
}
DynamicJsonDocument doc(64);
String state = "";
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
delay(1000);
client.poll();
    doc["TYPE"] = "cupUpdate";
    doc["CUP"] = cup;
    doc["ANALOG"] = analogValue;
    state = "";
    serializeJson(doc, state);
    // Serial.print("Sending: ");
    // Serial.println(state);
    client.send(state);

    if(singleClickBool){
      delay(100);
      lightControl(0x0000FF);
    client.send("{\"TYPE\":\"request\",\"CUP\":\"" + cup + "\"}");
    singleClickBool = false;
    }

    if(doubleClickBool){
      delay(100);
      lightControl(0xFF00FF);

    client.send("{\"TYPE\":\"calibrate\",\"CUP\":\"" + cup + "\"}");
    doubleClickBool = false;
    }
}
