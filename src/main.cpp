#include <Arduino.h>
#include <WiFi.h>

#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "Arduino_GigaDisplayTouch.h"
#include "Arduino_GigaDisplay.h"

#include "config.h"
#include "secrets.h"
#include "display.h"
#include "RotaryEncoder.h"
// #include "ui.h"
#include <TvControlClient.h>

#include "ui.h"

// extern const lv_img_dsc_t Abandonedfactory_small;

void gigaTouchHandler(uint8_t contacts, GDTpoint_t* points);

typedef struct {
  int brightness;
} Config;

auto cfg = (Config){
  100
};

Arduino_H7_Video          Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch  TouchDetector;
GigaDisplayBacklight backlight;


constexpr char ssid[] = WIFI_SSID;
constexpr char wifiPass[] = WIFI_PASS;

WiFiClient mqttWiFi;
MqttClient mqttClient(mqttWiFi);
String BedroomDimmerTopic = BEDROOM_DIMMER_TOPIC;
TvControlClient tv_controller(BEDROOM_TV_HOST, 8080, &mqttWiFi);
TvConfig tv_config;

Encoder LeftKnob(ROTARY_ENCODER_CLK_PIN, ROTARY_ENCODER_DATA_PIN, ROTARY_ENCODER_BTN_PIN);
volatile bool leftknob_turned = false;
void LeftKnobRotationCallback(long new_pos);
void HandleLeftKnobRotation(long pos);
volatile bool leftknob_clicked = false;
void HandleClickInput();

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Beginning initialization");
  Display.begin();
  TouchDetector.begin();

  Serial.println("Initializing encoder input");
  LeftKnob.register_btn_callback(&HandleClickInput);
  LeftKnob.register_rotation_callback(LeftKnobRotationCallback);
  LeftKnob.configure_bounds(0, 1);
  LeftKnob.begin(
    ROTARY_ENCODER_CLK_PIN,
    ROTARY_ENCODER_DATA_PIN,
    ROTARY_ENCODER_BTN_PIN);

  Serial.println("Initializing backlight controls");
  backlight.begin();
  backlight.set(cfg.brightness);

  TouchDetector.onDetect(gigaTouchHandler);

  while(WiFi.begin(ssid, wifiPass) != WL_CONNECTED) {
    // failed to connect
    Serial.println("Connecting to wifi...");
    delay(5000);
  }

  Serial.println("Connecting to MQTT broker...");
  if (!mqttClient.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT)) {
    Serial.print("MQTT connection failed! Error = ");
    Serial.println(mqttClient.connectError());
    while(true)
      // halt and catch fire
      ;
  }
  Serial.println("Ready!");

  ui_init();
}

bool updatedTvConfigRequested = true;
void loop() {
  // check for movement on the panel's dial
  const long leftknob_pos = LeftKnob.read();
  if (leftknob_clicked) {
    leftknob_clicked = false;
    Serial.println("encoder clicked");
  }
  if (leftknob_turned) {
    leftknob_turned = false;
    Serial.print("Left Knob turned to pos=");
    Serial.println(leftknob_pos);
    HandleLeftKnobRotation(leftknob_pos);
  }
  if (updatedTvConfigRequested) {
    updatedTvConfigRequested = false;
    Serial.println("Fetching updated TV config");
    tv_controller.FetchTvConfig(&tv_config);
    Serial.println("Got new playlist set:" );
    Serial.println(tv_config.playlist_options);
    lv_roller_set_options(ui_PlaylistSelectRoller, tv_config.playlist_options, LV_ROLLER_MODE_NORMAL);
    Serial.print("Selected Playlist: ");
    Serial.print(tv_config.current_playlist);
    Serial.print(" (");
    Serial.print(tv_config.index_of_playlist(tv_config.current_playlist));
    Serial.println(")");
    lv_roller_set_selected(ui_PlaylistSelectRoller, tv_config.index_of_playlist(tv_config.current_playlist), LV_ANIM_ON);
  }

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

// int currentScreen = 0;
// typedef void (*MenuRenderer)();
constexpr int SCREEN_COUNT = 2;
// MenuRenderer screens[SCREEN_COUNT] = {
//   display_menu_tv,
//   display_menu_lights
// };
//
void LeftKnobRotationCallback(long new_pos) {
  leftknob_turned = true;
}
void HandleLeftKnobRotation(const long pos) {

}

void HandleClickInput() {
  leftknob_clicked = true;
}

