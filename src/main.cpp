#include <Arduino.h>

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

const char ssid[] = WIFI_SSID;
const char wifiPass[] = WIFI_PASS;

WiFiClient mqttWiFi;
MqttClient mqttClient(mqttWiFi);
String BedroomDimmerTopic = BEDROOM_DIMMER_TOPIC;

void setup() {
  Display.begin();
  TouchDetector.begin();
  backlight.begin();
  backlight.set(cfg.brightness);
  TouchDetector.onDetect(gigaTouchHandler);

  while(WiFi.begin(ssid, wifiPass) != WL_CONNECTED) {
    // failed to connect
    Serial.println("Connecting to wifi...");
    delay(5000);
  }

  Serial.println("Success!");
  Serial.println("Connecting to MQTT broker...");
  if (!mqttClient.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT)) {
    Serial.print("MQTT connection failed! Error = ");
    Serial.println(mqttClient.connectError());
    while(true)
      // halt and catch fire
      ;
  }
  Serial.println("Success!");
  display_menu_lights();
}

void loop() {
  mqttClient.poll();
  lv_timer_handler();
  delay(10);
}

void display_main_menu() {
    //Display & Grid Setup
  lv_obj_t* screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, Display.width(), Display.height());

  static lv_coord_t col_dsc[] = { 370, 370, LV_GRID_TEMPLATE_LAST };
  static lv_coord_t row_dsc[] = { 215, 215, 215, 215, LV_GRID_TEMPLATE_LAST };

  lv_obj_t* grid = lv_obj_create(lv_scr_act());
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
  lv_obj_set_size(grid, Display.width(), Display.height());


  //top left
  lv_obj_t* obj;
  obj = lv_obj_create(grid);
  lv_obj_t* img;
  img = lv_img_create(obj);
  LV_IMG_DECLARE(Abandonedfactory_small);
  lv_img_set_src(img, &Abandonedfactory_small);
  lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(img, Abandonedfactory_small.header.w, Abandonedfactory_small.header.h);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                       LV_GRID_ALIGN_STRETCH, 0, 1);      //row

  //bottom left
  obj = lv_obj_create(grid);
  lv_obj_t* label;
  label = lv_label_create(obj);
  lv_label_set_text(label, "Hello, world!");

  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                       LV_GRID_ALIGN_STRETCH, 1, 1);      //row

  //top right
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 1, 1,  //column
                       LV_GRID_ALIGN_STRETCH, 0, 1);      //row


  //bottom right
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 1, 1,  //column
                       LV_GRID_ALIGN_STRETCH, 1, 1);      //row
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


