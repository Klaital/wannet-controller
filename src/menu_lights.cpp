//
// Created by Kit on 8/19/2024.
//

#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "Arduino_GigaDisplayTouch.h"
#include "display.h"
#include "menu_lights.h"

#include <WiFi.h>

void display_menu_lights() {
    //Display & Grid Setup
    lv_obj_t* screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, Display.width(), Display.height());

    static lv_coord_t col_dsc[] = { 370, 370, LV_GRID_TEMPLATE_LAST };
    static lv_coord_t row_dsc[] = { 215, 215, 215, 215, LV_GRID_TEMPLATE_LAST };

    lv_obj_t* grid = lv_obj_create(lv_scr_act());
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_size(grid, Display.width(), Display.height());

    //top left
    lv_obj_t *obj = lv_obj_create(grid);
    lv_obj_t *img = lv_img_create(obj);
    LV_IMG_DECLARE(Abandonedfactory_small);
    lv_img_set_src(img, &Abandonedfactory_small);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(img, Abandonedfactory_small.header.w, Abandonedfactory_small.header.h);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                         LV_GRID_ALIGN_STRETCH, 0, 1);      //row

    //bottom left
    lv_obj_t* lightsDimObj = lv_obj_create(grid);
    lv_obj_t* lightsDimLabel = lv_label_create(lightsDimObj);
    lv_label_set_text(lightsDimLabel, "Lights Dim");
    lv_obj_add_event_cb(lightsDimObj, lightsDimCallback, LV_EVENT_CLICKED, NULL);

    lv_obj_set_grid_cell(lightsDimObj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                         LV_GRID_ALIGN_STRETCH, 1, 1);      //row

    //top right
    lv_obj_t* lightsOnObj = lv_obj_create(grid);
    lv_obj_t* lightsOnLabel = lv_label_create(lightsOnObj);
    lv_label_set_text(lightsOnLabel, "Lights On");
    lv_obj_add_event_cb(lightsOnObj, lightsOnCallback, LV_EVENT_CLICKED, NULL);
    lv_obj_set_grid_cell(lightsOnObj, LV_GRID_ALIGN_STRETCH, 1, 1,  //column
                         LV_GRID_ALIGN_STRETCH, 0, 1);      //row

    //bottom right
    lv_obj_t* lightsOffObj = lv_obj_create(grid);
    lv_obj_t* lightsOffLabel = lv_label_create(lightsOffObj);
    lv_label_set_text(lightsOffLabel, "Lights Off");
    lv_obj_add_event_cb(lightsOffObj, lightsOffCallback, LV_EVENT_CLICKED, NULL);

    lv_obj_set_grid_cell(lightsOffObj, LV_GRID_ALIGN_STRETCH, 1, 1,  //column
                         LV_GRID_ALIGN_STRETCH, 1, 1);      //row
}

void lightsDimCallback(lv_event_t * e) {
    lv_obj_t * button = (lv_obj_t*)lv_event_get_target(e);
    lv_obj_t * label = (lv_obj_t*)lv_obj_get_child(button, 0);
    lv_label_set_text_fmt(label, "Lights Power 15%%");
    Serial.print("Dimming lights on topic ");
    Serial.print(BedroomDimmerTopic);
    Serial.print(": ");
    Serial.println("0.15");

    mqttClient.beginMessage(BedroomDimmerTopic);
    mqttClient.print("0.15");
    mqttClient.endMessage();
}

void lightsOnCallback(lv_event_t* e) {
    lv_obj_t * button = (lv_obj_t*)lv_event_get_target(e);
    lv_obj_t * label = (lv_obj_t*)lv_obj_get_child(button, 0);
    lv_label_set_text_fmt(label, "Lights Power 100%%");
    Serial.println("Turning lights on");
    mqttClient.beginMessage(BedroomDimmerTopic);
    mqttClient.print("1.0");
    mqttClient.endMessage();
}

void lightsOffCallback(lv_event_t* e) {
    lv_obj_t * button = (lv_obj_t*)lv_event_get_target(e);
    lv_obj_t * label = (lv_obj_t*)lv_obj_get_child(button, 0);
    lv_label_set_text_fmt(label, "Lights Power 0%%");
    Serial.println("Turning lights off");
    mqttClient.beginMessage(BedroomDimmerTopic);
    mqttClient.print("0.0");
    mqttClient.endMessage();
}
