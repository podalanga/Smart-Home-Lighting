#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

const char* ssid = "Podalanga";
const char* password = "meowmeow";

AsyncWebServer server(80);

// RGB & switch state variables
int r_val = 0, g_val = 0, b_val = 0;
float h = 0, s = 0, v = 0;
int led_state = 0;
int ambient_state = 0;
const int main_switch=22;
const int ldr=36;
int main_switch_state=0;
const int r_led=32;
const int g_led=33;
const int b_led=25;
const int freq=5000;
const int resolution=8;
const int led_channel[] = {0, 1, 2};
const char url_name[] = "podalanga";
int ldr_val=0;

int check_switch();
void init();
int check_switch();
void HSV2RGB (float H, float S, float V, int &R, int &G, int &B);


void setup() {
  Serial.begin(115200);
  // Init SPIFFS
  if(!SPIFFS.begin(true)) {
    // Serial.println("SPIFFS failed");
    return;
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected: " + WiFi.localIP().toString());
  
  if (!MDNS.begin("podalanga")) {
    // Serial.println("Error starting mDNS");
    return;
  }
  // Serial.printf("mDNS started. You can access the server at http://%s.local\n", url_name);


  // Serve the HTML file
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (SPIFFS.exists("/index.html")) {
      // Serial.println("Serving index.html from SPIFFS");
      request->send(SPIFFS, "/index.html", "text/html");
    } else {
      // Serial.println("index.html not found!");
      request->send(404, "text/plain", "File not found");
    }
  });
  // Receive HSV values
  server.on("/sethsv", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("h")) h= request->getParam("h")->value().toFloat();
    if (request->hasParam("s")) s = request->getParam("s")->value().toFloat();
    if (request->hasParam("v")) v = request->getParam("v")->value().toFloat();
    if (request->hasParam("h") && request->hasParam("s") && request->hasParam("v")) {
        // Serial.printf("Received HSV params: h=%.2f, s=%.2f, v=%.2f\n", h, s, v);
    } else {
        // Serial.println("Missing HSV parameters");
    }
    // Serial.printf("Color: h=%f, s=%f, v=%f\n", h, s, v);
    request->send(200, "text/plain", "OK");
  });

  // LED toggle endpoint
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request){
    String state = request->getParam("state")->value();
    led_state = (state == "on") ? 1:0;
    // Serial.printf("LED: %s\n", led_state ? "ON" : "OFF");
    request->send(200, "text/plain", "LED toggled");
  });

  // Ambient toggle endpoint
  server.on("/ambient", HTTP_GET, [](AsyncWebServerRequest *request){
    String state = request->getParam("state")->value();
    ambient_state = (state == "on") ? 1:0;
    // Serial.printf("Ambient: %s\n", ambient_state ? "ON" : "OFF");
    request->send(200, "text/plain", "Ambient toggled");
  });

  server.begin();
  init();
  // Serial.println("Server started");
}
void loop() {
  ldr_val=analogRead(ldr);
  if (check_switch() && led_state)
  {
    if (ambient_state && ldr_val < 2700) 
    {
      v=(2700-ldr_val)/2700.0;
      if (v>1){
        v=1;
      }
      HSV2RGB(h, s, v, r_val, g_val, b_val);
    }
    else
    {
      HSV2RGB(h, s, v, r_val, g_val, b_val);
    }
    ledcWrite(led_channel[0], r_val); 
    ledcWrite(led_channel[1], g_val);
    ledcWrite(led_channel[2], b_val);
  }
  else {
    ledcWrite(led_channel[0], 0);  // Turn off all LEDs
    ledcWrite(led_channel[1], 0);
    ledcWrite(led_channel[2], 0);
  }

}

void init() {
  ledcSetup(led_channel[0], freq, resolution);
  ledcSetup(led_channel[1], freq, resolution);
  ledcSetup(led_channel[2], freq, resolution);
  ledcAttachPin(r_led, led_channel[0]);
  ledcAttachPin(g_led, led_channel[1]);
  ledcAttachPin(b_led, led_channel[2]);
  pinMode(main_switch, INPUT_PULLDOWN);
  pinMode(main_switch, INPUT);
}

int check_switch() {
  int current_state = digitalRead(main_switch);
  if (current_state== LOW) {
    main_switch_state = 0;
    return 0;
  } else {
    main_switch_state = 1;
    return 1;
  }
}

void HSV2RGB (float H, float S, float V, int &R, int &G, int &B) {
  float C = V * S;
  float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
  float m = V - C;

  if (H >= 0 && H < 60) {
    R = (C + m) * 255;
    G = (X + m) * 255;
    B = m * 255;
  } else if (H >= 60 && H < 120) {
    R = (X + m) * 255;
    G = (C + m) * 255;
    B = m * 255;
  } else if (H >= 120 && H < 180) {
    R = m * 255;
    G = (C + m) * 255;
    B = (X + m) * 255;
  } else if (H >= 180 && H < 240) {
    R = m * 255;
    G = (X + m) * 255;
    B = (C + m) * 255;
  } else if (H >= 240 && H < 300) {
    R = (X + m) * 255;
    G = m * 255;
    B = (C + m) * 255;
  } else {
    R = (C + m) * 255;
    G = m * 255;
    B = (X + m) * 255;
  }
}

