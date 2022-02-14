#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

#include <ArduinoJson.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_MOSI D7  // Connect to D1 on OLED
#define OLED_CLK D5   // Connect to D0 on OLED
#define OLED_DC D1    // Connect to DC on OLED
#define OLED_CS D8    // Connect to CS on OLED
#define OLED_RESET D3 // Connect to RES on OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#define NUMFLAKES 10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

int BUZZER = D2;

const char *ssid = "VibinBar";     // The SSID (name) of the Wi-Fi network you want to connect to
const char *password = "vibinbar"; // The password of the Wi-Fi network
AsyncWebServer httpServer(80);

AsyncWebSocket ws("/ws");

int A_Percentage = 0;
int B_Percentage = 0;

unsigned long statusTime = 0;

void buzzerNotification()
{
  analogWrite(BUZZER, 64);
  delay(100);
  analogWrite(BUZZER, 0);
  delay(100);
  analogWrite(BUZZER, 64);
  delay(100);
  analogWrite(BUZZER, 0);
}

void testdrawchar(void)
{
  display.clearDisplay();

  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.cp437(true);                 // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for (int16_t i = 0; i < 256; i++)
  {
    if (i == '\n')
      display.write(' ');
    else
      display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void)
{
  display.clearDisplay();

  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x"));
  display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void testscrolltext(void)
{
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display(); // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}
bool statusDisplay = false;
void displayText(String text)
{
  Serial.print("GOT HERE");
  // yield();
  delay(200);
  display.clearDisplay();
  display.setTextSize(1); // Normal 1:1 pixel scale

  display.setTextColor(SSD1306_WHITE); // Draw white text

  display.setCursor(0, 0); // Start at top-left corner

  display.println(text.c_str());
  delay(200);
  // yield();
  display.display();
  // yield();
  delay(200);

  statusTime = millis();
  statusDisplay = true;
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  // About 160 messages can be sent before it dies
  // Take data and put it in JSON
  if (type == WS_EVT_CONNECT)
  {

    Serial.println("Websocket client connection received");
    client->text("Hello from Vibin"); // Don't really need a hello message
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.println("Client disconnected");
  }
  else if (type == WS_EVT_DATA)
  {
    // data packet
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len)
    {
      // the whole message is in a single frame and we got all of it's data
      os_printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);
      if (info->opcode == WS_TEXT)
      {
        data[len] = 0;
        os_printf("%s\n", (char *)data);
      }
      else
      {
        for (size_t i = 0; i < info->len; i++)
        {
          os_printf("%02x ", data[i]);
        }
        os_printf("\n");
      }
      if (info->opcode == WS_TEXT)
      {
        StaticJsonDocument<64> doc;

        DeserializationError error = deserializeJson(doc, (char *)data, len);

        if (error)
        {
          client->text("{\"status\": false, \"TYPE\":\"status\"}");
        }
        else
        {
          // Serial.print(data)
          const char *TYPE = doc["TYPE"];
          if (strcmp(TYPE, "newPatron") == 0)
          {
            Serial.println("New Patron");
            String name = doc["NAME"];
            String drink = doc["DRINK"];
            String cup = doc["CUP"];
            // concatenate the name and drink
            String patron = name + " - " + drink + " - " + cup;
            // Serial.println(patron);

            // display.clearDisplay();
            // display.setTextSize(1);             // Normal 1:1 pixel scale

            // display.setTextColor(SSD1306_WHITE);        // Draw white text

            // display.setCursor(0,0);             // Start at top-left corner

            // display.println(name.c_str());
            // display.setCursor(1,0);             // Start at mid-left corner
            // display.println(drink.c_str());
            // display.setCursor(2,0);             // Start at bottom-left corner
            // String cupString = "Cup: " + cup;
            // display.println(cupString.c_str());
            // display.display();      // Show text

            displayText(patron);
            buzzerNotification();

            ws.textAll("{\"TYPE\": \"cupMessage\", \"CUP\": \"" + cup + "\"}");
          }
          else if (strcmp(TYPE, "newDrink") == 0)
          {
            Serial.println("New Drink");
            String name = doc["NAME"];
            String drink = doc["DRINK"];
            String cup = doc["CUP"];
            // concatenate the name and drink
            String patron = name + " - " + drink + " - " + cup;
            // Serial.
          }
          else if (strcmp(TYPE, "percentage") == 0)
          {
            // Serial.println("Percentage");
            String name = doc["NAME"];
            String cup = doc["CUP"];
            int percentage = doc["PERCENTAGE"];
            if (strcmp(cup.c_str(), "A") == 0)
            {
              A_Percentage = percentage;
            }
            else
            {
              B_Percentage = percentage;
            }
            DynamicJsonDocument old(64);
            String state = "";
            old["TYPE"] = "percentage";
            old["CUP"] = cup;
            old["PERCENTAGE"] = doc["PERCENTAGE"];
            ;
            state = "";
            serializeJson(old, state);
            ws.textAll(state);
          }

          else if (strcmp(TYPE, "cupUpdate") == 0)
          {

            DynamicJsonDocument old(64);
            String state = "";
            old["TYPE"] = "cupUpdate";
            old["CUP"] = doc["CUP"];
            old["ANALOG"] = doc["ANALOG"];
            state = "";
            serializeJson(old, state);
            ws.textAll(state);
          }
          else if (strcmp(TYPE, "request") == 0)
          {

            DynamicJsonDocument old(64);
            String state = "";
            old["TYPE"] = "request";
            old["CUP"] = doc["CUP"];
            state = "";
            serializeJson(old, state);
            ws.textAll(state);
            String cup = doc["CUP"];
            displayText("Cup " + cup + " requested assistance.");
            buzzerNotification();

            // concatenate the name and cup
          }
          else if (strcmp(TYPE, "refill") == 0)
          {
            String name = doc["NAME"];
            String cup = doc["CUP"];
            String drink = doc["DRINK"];
            displayText(name + " needs " + drink + " for cup " + cup);
            buzzerNotification();
          }
          else if (strcmp(TYPE, "calibrate") == 0)
          {

            DynamicJsonDocument old(64);
            String state = "";
            old["TYPE"] = "calibrate";
            old["CUP"] = doc["CUP"];
            state = "";
            serializeJson(old, state);
            ws.textAll(state);
          }

          client->text("{\"status\": true, \"TYPE\":\"status\"}"); // needs to have some kind fo check /timer that if you don't get a command from (ping pong) to auto turn off
        }
      }
      else
        client->binary("I got your binary message");
    }
    else
    {
      // message is comprised of multiple frames or the frame is split into multiple packets
      if (info->index == 0)
      {
        if (info->num == 0)
          os_printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
        os_printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      os_printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);
      if (info->message_opcode == WS_TEXT)
      {
        data[len] = 0;
        os_printf("%s\n", (char *)data);
      }
      else
      {
        for (size_t i = 0; i < len; i++)
        {
          os_printf("%02x ", data[i]);
        }
        os_printf("\n");
      }

      if ((info->index + len) == info->len)
      {
        os_printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if (info->final)
        {
          os_printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
          if (info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
  // Handle JSON stuff here
}

void statusScreen()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("  A    B  ");
  display.setTextSize(1);

  display.print("    " + String(A_Percentage) + "%" + "       " + String(B_Percentage) + "%");
  display.display();
}

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  pinMode(BUZZER, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  // display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  displayText("Attempting to connect to WiFi");
IPAddress local_IP(192, 168, 137, 2);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);

  WiFi.config(local_IP, gateway, subnet);


  WiFi.begin(ssid, password); // Connect to the network
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(1000);
    yield();
    // Serial.println(++i);
    // Serial.println(WiFi.status());
    //  Serial.println(' ');
    Serial.println(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
  displayText("Connected to WiFi      " + WiFi.localIP().toString());
  ws.onEvent(onWsEvent);
  httpServer.addHandler(&ws);

  httpServer.begin();
  // testdrawchar();      // Draw characters of the default font

  // testdrawstyles();    // Draw 'stylized' characters

  // testscrolltext();    // Draw scrolling text

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
    statusScreen();
  buzzerNotification();
}

void loop()
{
  if (statusDisplay && millis() - statusTime > 15000)
  {
    statusScreen();
    statusDisplay = false;
  }
}
