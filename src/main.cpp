#include <Arduino.h>
#include <Arduino_GigaDisplayTouch.h>
#include <Arduino_H7_Video.h>
#include <Arduino_GigaDisplay.h>
#include <WiFi.h>

#include <config.h>
#include <secrets.h>
#include <ui.h>
#include <AlarmClock.h>
#include <Encoder.h>
#include <HttpClient.h>
#include <mbed_mktime.h>
#include <MqttClient.h>
#include <Request.h>
#include <Response.h>
#include <TvConfig.h>
#include <TvControlClient.h>

Arduino_GigaDisplayTouch  TouchDetector;
Arduino_H7_Video Display(800, 480, GigaDisplayShield);

// TODO: Configs to persist between power cycles
int BacklightBrightness = 100;
int TimezoneOffset = -7;
int WakeupTime = (6 * 3600) + (30 * 60); // 6:30am

// Backlight switch
GigaDisplayBacklight Backlight;
volatile bool backlight_switch_changed = true;
void HandleBacklightSwitch();
void BacklightSwitchISR();

// Rotary Encoder dial
Encoder LeftKnob(ROTARY_ENCODER_CLK_PIN, ROTARY_ENCODER_DATA_PIN, ROTARY_ENCODER_BTN_PIN);
volatile bool leftknob_turned = false;
volatile bool leftknob_clicked = false;
void LeftKnobRotationCallback(long new_pos);
void HandleLeftKnobRotation(long pos);
void HandleClickInput();

// WiFi
int ConnStatus;
WiFiClient net;

// Alarm Clock
Wan::AlarmClock alarms(TimezoneOffset);
HTTP::Request wakeup_request;
volatile bool wakeup_requested = false;
void DoWakeup(const tm& now);
void UpdateClock(const tm& now);

// Lights Controller
HttpClient lights_client(LIGHTS_CONTROLLER_HOST, 80, &net);
MqttClient mqttClient(net);
volatile bool change_lights_requested = false;
void SetLightsBrightness();

// TV Remote Control
TvControlClient tv_controller(BEDROOM_TV_HOST, 8080, &net);
TvConfig tv_config;
volatile bool update_tv_config_requested = true;
volatile bool change_playlist_requested = false;


void setup() {
    Serial.begin(9600);
    Display.begin();
    TouchDetector.begin();
    Backlight.off();

    // Initialize the backlight switch
    pinMode(BACKLIGHT_SWITCH_ON_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BACKLIGHT_SWITCH_ON_PIN), BacklightSwitchISR, CHANGE);

    // Initialize the rotary encoder
    LeftKnob.register_btn_callback(HandleClickInput);
    LeftKnob.register_rotation_callback(LeftKnobRotationCallback);
    LeftKnob.configure_bounds(0, 2);
    LeftKnob.begin(
      ROTARY_ENCODER_CLK_PIN,
      ROTARY_ENCODER_DATA_PIN,
      ROTARY_ENCODER_BTN_PIN);

    // Connect to wifi
    ConnStatus = WiFi.begin(WIFI_SSID, WIFI_PASS);
    while(ConnStatus != WL_CONNECTED) {
        delay(5000);
        Serial.println("Failed to connect. Retrying...");
        ConnStatus = WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    // Set up the clock.
    rtc_init();
    // Use NTP from the wifi module as the clock time.
    alarms.set_alarm(WakeupTime, DoWakeup);
    alarms.add_tick_handler(UpdateClock);
    strcpy(wakeup_request.path, "/lights/wakeup");
    strcpy(wakeup_request.method, "PUT");


    // Start up the UI
    ui_init();
    Backlight.begin();
    backlight_switch_changed = true; // force evaluation of the switch position on the first loop
}

void loop() {
    // Poll devices
    const long leftknob_pos = LeftKnob.read();

    // Handle ISR requests
    if (backlight_switch_changed) {
        HandleBacklightSwitch();
    }
    if (leftknob_clicked) {
        leftknob_clicked = false;
        Serial.println("encoder clicked. Nothing is connected to this input yet.");
    }
    if (leftknob_turned) {
        leftknob_turned = false;
        HandleLeftKnobRotation(leftknob_pos);
    }
    if (wakeup_requested) {
        wakeup_requested = false;
        tm now;
        _rtc_localtime(time(nullptr), &now, RTC_4_YEAR_LEAP_YEAR_SUPPORT);
        DoWakeup(now);
    }
    if (update_tv_config_requested) {
        update_tv_config_requested = false;
        Serial.println("Fetching updated TV config...");
        if (tv_controller.FetchTvConfig(&tv_config)) {
            lv_roller_set_options(ui_PlaylstSelection, tv_config.playlist_options, LV_ANIM_ON);
            Serial.print("Selected playlist: ");
            Serial.println(tv_config.current_playlist);
        }
    }
    if (change_playlist_requested) {
        change_playlist_requested = false;
        Serial.print("Changing playlist to ");
        char playlist_name[64] = "";
        lv_roller_get_selected_str(ui_PlaylstSelection, playlist_name, 64);
        Serial.println(playlist_name);
        tv_controller.ChangePlaylist(playlist_name);
    }
    if (change_lights_requested) {
        change_lights_requested = false;
        Serial.println("Submitting updated light power...");
        SetLightsBrightness();
    }

    alarms.tick();
    lv_timer_handler();
    delay(10);

    // call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqttClient.poll();
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
void DoWakeup(const tm& now) {
    Serial.println("Good morning!");
    // send HTTP request to turn on the lights
    HTTP::Response resp;
    lights_client.exec(wakeup_request, resp);
    if (resp.code != 204) {
        Serial.print("Error starting wakeup: ");
        Serial.print(resp.code);
        Serial.print(" ");
        Serial.println(resp.status);
    }
}

void UpdateClock(const tm& now) {
    // TODO: add date to clock
    lv_label_set_text_fmt(ui_Clock, "%02d-%02d\n%02d:%02d:%02d", now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
}
void SetLightsBrightness() {
    Serial.println("Updating lights");
    const auto pct = lv_slider_get_value(ui_LightsDimmer);
    const float pwr = static_cast<float>(pct) / 100.0f;

    // send brightness update via mqtt
    mqttClient.beginMessage(BEDROOM_DIMMER_TOPIC);
    mqttClient.print(pwr);
    mqttClient.endMessage();
}
