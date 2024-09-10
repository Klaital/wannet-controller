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

#include "mbed.h"
#include <mbed_mktime.h>

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
GigaDisplayRGB rgb; // controls the RGB LED on the Display
void DisplayError(const char* msg);


constexpr char ssid[] = WIFI_SSID;
constexpr char wifiPass[] = WIFI_PASS;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
String BedroomDimmerTopic = BEDROOM_DIMMER_TOPIC;
TvControlClient tv_controller(BEDROOM_TV_HOST, 8080, &wifiClient);
TvConfig tv_config;

Encoder LeftKnob(ROTARY_ENCODER_CLK_PIN, ROTARY_ENCODER_DATA_PIN, ROTARY_ENCODER_BTN_PIN);
volatile bool leftknob_turned = false;
void LeftKnobRotationCallback(long new_pos);
void HandleLeftKnobRotation(long pos);
volatile bool leftknob_clicked = false;
void HandleClickInput();

// Wakeup scheduling
void CheckScheduledEvents();
void rtc_from_ntp();
void set_wakeup_time(int hour, int minute);
HTTP::Request wakeup_request;
HttpClient lights_client(LIGHTS_CONTROLLER_HOST, 80, &wifiClient);

void setup() {
  Serial.begin(9600);
  Serial.println("Beginning initialization");
  Display.begin();
  TouchDetector.begin();
  rgb.begin();
  rgb.on(0, 0, 255);


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

#ifdef DEBUG
  while(!Serial);
#endif

  int conn_attempts = 0;
  while(WiFi.begin(ssid, wifiPass) != WL_CONNECTED) {
    // failed to connect
    Serial.println("Connecting to wifi...");
    conn_attempts += 1;
    if (conn_attempts > 5) {
      DisplayError("Failed to connect to wifi");
    }
    delay(5000);
  }

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Start initializing the clock from network time
  rtc_init();
  rtc_from_ntp();
  set_wakeup_time(14, 15);
  strcpy(wakeup_request.path, "/lights/wakeup");
  strcpy(wakeup_request.method, "PUT");


  Serial.println("Connecting to MQTT broker...");
  if (!mqttClient.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT)) {
    Serial.print("MQTT connection failed! Error = ");
    Serial.println(mqttClient.connectError());
    // TODO: handle automatically reconnecting to MQTT after a network or server outage
    // while(true)
    //   // halt and catch fire
    //   ;
  }
  Serial.println("Ready!");

  ui_init();
  rgb.off();
}

bool updatedTvConfigRequested = true;
bool changePlaylistRequested = false;
void loop() {
  // check for movement on the panel's dial
  const long leftknob_pos = LeftKnob.read();
  if (leftknob_clicked) {
    leftknob_clicked = false;
    Serial.println("encoder clicked. Nothing is connected to this input yet.");
  }
  if (leftknob_turned) {
    leftknob_turned = false;
    HandleLeftKnobRotation(leftknob_pos);
  }
  if (updatedTvConfigRequested) {
    updatedTvConfigRequested = false;
    Serial.println("Fetching updated TV config");
    if (tv_controller.FetchTvConfig(&tv_config)) {
      lv_roller_set_options(ui_PlaylistSelectRoller, tv_config.playlist_options, LV_ROLLER_MODE_NORMAL);
      Serial.print("Selected Playlist: ");
      Serial.println(tv_config.current_playlist);
      lv_roller_set_selected(ui_PlaylistSelectRoller, tv_config.index_of_playlist(tv_config.current_playlist), LV_ANIM_ON);
    }
  }
  if (changePlaylistRequested) {
    changePlaylistRequested = false;
    Serial.print("Changing playlist to ");
    char playlist_name[64] = "";
    lv_roller_get_selected_str(ui_PlaylistSelectRoller, playlist_name, 64);
    Serial.println(playlist_name);
    tv_controller.ChangePlaylist(playlist_name);
  }

  // mqttClient.poll();
  CheckScheduledEvents();
  rtc_from_ntp();
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

void LeftKnobRotationCallback(long new_pos) {
  leftknob_turned = true;
}
void HandleLeftKnobRotation(const long pos) {
  // Switch tabs when the left knob is turned.
  lv_tabview_set_act(ui_TabView1, pos, LV_ANIM_ON);
}

void HandleClickInput() {
  leftknob_clicked = true;
}

void WakeupJob() {
  Serial.println("Requesting lights to start wakeup routing");
}

// // Code to run at the same time each day
// ScheduledJob scheduled_jobs[] = {
//   WakeupJob
// };
//
// // The time of day to run each job. Expressed as seconds since midnight.
// unsigned long job_schedules[] = {
//   19800 // 5:30 am
// };
//
// // The time of the job's last run. Used to debounce the execution.
// unsigned long job_last_run[] = {
//   0
// };
//
// void CheckScheduledJobs() {
//
// }
