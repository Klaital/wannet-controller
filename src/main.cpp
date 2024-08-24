#include <Arduino.h>
#include <RotaryEncoder.h>

#include <WiFi.h>
#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "Arduino_GigaDisplayTouch.h"
#include "Arduino_GigaDisplay.h"
#include "display.h"
#include "menu_lights.h"
#include "config.h"
#include "secrets.h"

extern const lv_img_dsc_t Abandonedfactory_small;

void gigaTouchHandler(uint8_t contacts, GDTpoint_t* points);


typedef struct {
  int brightness;
} Config;
Config cfg = (Config){
  100
};

Arduino_H7_Video          Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch  TouchDetector;
GigaDisplayBacklight backlight;
extern RotaryEncoder Encoder;
volatile bool encoderClicked = false;
volatile bool encoderClocked = false;


const char ssid[] = WIFI_SSID;
const char wifiPass[] = WIFI_PASS;

WiFiClient mqttWiFi;
MqttClient mqttClient(mqttWiFi);
String BedroomDimmerTopic = BEDROOM_DIMMER_TOPIC;

void HandleClockwiseTurn();
void HandleClickInput();
void HandleCounterClockwiseTurn();

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Beginning initialization");
  Display.begin();
  TouchDetector.begin();

  Serial.println("Initializing encoder input");
  Encoder.register_btn_handler(&HandleClickInput);
  Encoder.register_cw_handler(&HandleClockwiseTurn);
  Encoder.register_ccw_handler(&HandleCounterClockwiseTurn);
  Encoder.begin(
    ROTARY_ENCODER_CLK_PIN,
    ROTARY_ENCODER_DATA_PIN,
    ROTARY_ENCODER_BTN_PIN);

  Serial.println("Initializing backlight controls");
  backlight.begin();
  backlight.set(cfg.brightness);
  TouchDetector.onDetect(gigaTouchHandler);

  // while(WiFi.begin(ssid, wifiPass) != WL_CONNECTED) {
  //   // failed to connect
  //   Serial.println("Connecting to wifi...");
  //   delay(5000);
  // }
  //
  // Serial.println("Success!");
  // Serial.println("Connecting to MQTT broker...");
  // if (!mqttClient.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT)) {
  //   Serial.print("MQTT connection failed! Error = ");
  //   Serial.println(mqttClient.connectError());
  //   while(true)
  //     // halt and catch fire
  //     ;
  // }
  Serial.println("Ready!");
  display_menu_lights();
}

void loop() {
  // if (encoderClicked) {
  //   encoderClicked = false;
  //   Serial.println("encoder clicked");
  // }
  // if (encoderClocked) {
  //   encoderClocked = false;
  //   Encoder.handle_rising_clk();
  // }
  // check for movement on the panel's dial
  Encoder.poll();

  // mqttClient.poll();
  lv_timer_handler();
  delay(10);
}

unsigned long lastTouch;
unsigned long touchThreshold = 250; // milliseconds
void gigaTouchHandler(const uint8_t contacts, GDTpoint_t* points) {
  if (millis() - lastTouch < touchThreshold) {
    // no-op until the cooldown has expired
    return;
  }
  if (contacts <= 0) {
    // no-op if there's no touches
    return;
  }
  // Serial.print("Contacts: ");
  // Serial.println(contacts);
  //
  // /* First touch point */
  // Serial.print(points[0].x);
  // Serial.print(" ");
  // Serial.println(points[0].y);
}

void HandleClockwiseTurn() {
  Serial.println("Turned Clockwise");
}
void HandleCounterClockwiseTurn() {
  Serial.println("Turned CounterClockwise");
}

void HandleClickInput() {
  Serial.println("Button clicked");
}