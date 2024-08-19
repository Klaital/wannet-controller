#include <Arduino.h>

#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "Arduino_GigaDisplayTouch.h"
#include "Arduino_GigaDisplay.h"

extern const lv_img_dsc_t Abandonedfactory_small;

void gigaTouchHandler(uint8_t contacts, GDTpoint_t* points);
static void lightsDimCallback(lv_event_t * e);
void display_lights_menu();

typedef struct {
  int brightness;
} Config;
Config cfg = (Config){
  100
};

Arduino_H7_Video          Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch  TouchDetector;
GigaDisplayBacklight backlight;

void setup() {
  Display.begin();
  TouchDetector.begin();
  backlight.begin();
  backlight.set(cfg.brightness);
  TouchDetector.onDetect(gigaTouchHandler);
  display_lights_menu();
}

void loop() {
  // put your main code here, to run repeatedly:
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

void display_lights_menu() {
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
  lv_obj_t* lightsDimLabel;
  lv_obj_t* lightsDimButton;
  lightsDimButton = lv_button_create(obj);
  lv_obj_set_size(lightsDimButton, 200, 100);
  lightsDimLabel = lv_label_create(lightsDimButton);
  lv_label_set_text(lightsDimLabel, "Dim Lights");
  lv_obj_add_event_cb(lightsDimButton, lightsDimCallback, LV_EVENT_CLICKED, NULL);

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
void gigaTouchHandler(uint8_t contacts, GDTpoint_t* points) {
  if (millis() - lastTouch < touchThreshold) {
    // no-op until the cooldown has expired
    return;
  }
  if (contacts <= 0) {
    // no-op if there's no touches
    return;
  }
  Serial.print("Contacts: ");
  Serial.println(contacts);

  /* First touch point */
  Serial.print(points[0].x);
  Serial.print(" ");
  Serial.println(points[0].y);
}


static void lightsDimCallback(lv_event_t * e) {
  lv_obj_t * button = (lv_obj_t*)lv_event_get_target(e);
  lv_obj_t * label = (lv_obj_t*)lv_obj_get_child(button, 0);
  lv_label_set_text_fmt(label, "Clicked!");
}