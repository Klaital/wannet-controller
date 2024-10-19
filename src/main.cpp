#define DEBUG 1

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
int NewWakeupTime = WakeupTime;
int LightsOffTime = (9 * 3600); // 9am

// Backlight switch
GigaDisplayBacklight Backlight;
volatile bool backlight_switch_changed = true;
void HandleBacklightSwitch();
void BacklightSwitchISR();

// Rotary Encoder dial
Encoder LeftKnob(ROTARY_ENCODER_CLK_PIN, ROTARY_ENCODER_DATA_PIN, ROTARY_ENCODER_BTN_PIN);
volatile int leftknob_turned = 0;

volatile bool leftknob_clicked = true;
long old_pos = 0;
void LeftKnobRotationCallback(long new_pos, int direction);
void HandleLeftKnobRotation(int direction);
void HandleClickInput();

// WiFi
int ConnStatus;
unsigned long WiFiReconnectTime = 0; // used to space out reconnect attempts
WiFiClient net;
unsigned long http_error_time = 0;
void handle_net_error(HTTP::Response *resp);
void handle_http_error(HTTP::Response *resp);

// Alarm Clock
Wan::AlarmClock alarms(TimezoneOffset);
// HTTP::Request wakeup_request;
// HTTP::Request lightsout_request;
volatile bool wakeup_requested = false;
void DoWakeup(const tm& now);
void DoLightsOut(const tm& now);
void UpdateClock(const tm& now);
void rtc_from_ntp(); // updates the onboard RTC from an NTC server. Will only query NTP once per day.

// Lights Controller
// HttpClient lights_client(LIGHTS_CONTROLLER_HOST, 80, &net);
MqttClient mqttClient(net);
void handleMqttMessage(int messageSize);
volatile int mqtt_status = 0;
volatile bool change_lights_requested = false;
void SetLightsBrightness();

// TV Remote Control
// TvControlClient tv_controller(BEDROOM_TV_HOST, 8080, &net);
TvConfig tv_config;
void UpdateNowPlaying(const char *title);
void RequestTvConfigs();
void HandleTvConfig(const String& raw);

volatile bool update_tv_config_requested = true;
volatile bool change_playlist_requested = false;

// Buzzer
void chirp();

void setup() {
    Serial.begin(9600);
    Display.begin();
    TouchDetector.begin();
    Backlight.off();

    // initilize the buzzer
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

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

    // Set up the clock.
    rtc_init();
    // Use NTP from the wifi module as the clock time.
    rtc_from_ntp();

    alarms.set_alarm("wakeup", WakeupTime, DoWakeup);
    alarms.set_alarm("lights off", LightsOffTime, DoLightsOut);
    // alarms.add_tick_handler(UpdateClock);
    // strcpy(wakeup_request.path, "/lights/wakeup");
    // strcpy(wakeup_request.method, "PUT");
    // strcpy(lightsout_request.path, "/lights/?state=off");
    // strcpy(lightsout_request.method, "PUT");


    // Start up the UI
    ui_init();
    alarms.configure_lvgl_digital_clock(ui_Clock);
    alarms.configure_lvgl_countdowns(ui_WakeupCountdown);

    Backlight.begin();
    backlight_switch_changed = true; // force evaluation of the switch position on the first loop

    // Register UI handler callbacks
    // lights_client.net_error_callback = handle_net_error;
    // lights_client.resp_callback = handle_http_error;
    // tv_controller.RegisterHandlers(handle_net_error, handle_http_error);
    lv_label_set_text_fmt(ui_lblWiFiStatus, "%d", ConnStatus);

    mqttClient.subscribe(BEDROOM_CONFIG_TOPIC);
    mqttClient.subscribe(BEDROOM_NOWPLAYING_TOPIC);
    mqttClient.onMessage(handleMqttMessage);
}

void loop() {
    const auto now_ms = millis();
    // Connect to wifi if needed
    if (ConnStatus != WL_CONNECTED && now_ms - WiFiReconnectTime > 5000) {
        Serial.println("Connecting to wifi...");
        ConnStatus = WiFi.begin(WIFI_SSID, WIFI_PASS);
        lv_label_set_text_fmt(ui_lblWiFiStatus, "%d", ConnStatus);
        if(ConnStatus != WL_CONNECTED) {
            Serial.print("Failed to connect: ");
            Serial.println(ConnStatus);
        }
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    }
    if (ConnStatus == WL_CONNECTED && !mqtt_status) {
        // Reconnect to mqtt broker
        mqtt_status = mqttClient.connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT);
        if (!mqtt_status) {
            Serial.print("Failed to connect to mqtt broker ");
            Serial.print(MQTT_BROKER_HOST);
            Serial.print(":");
            Serial.print(MQTT_BROKER_PORT);
            Serial.print(" - ");
            Serial.println(mqttClient.connectError());
        } else {
            Serial.println("Connected to MQTT broker");
            mqttClient.subscribe(BEDROOM_CONFIG_TOPIC);
            mqttClient.subscribe(BEDROOM_NOWPLAYING_TOPIC);
            mqttClient.onMessage(handleMqttMessage);
        }
    }
    // Clear the HTTP error display after a timeout
    if (http_error_time > 0 && now_ms - http_error_time > 10000) {
        http_error_time = 0;
        lv_label_set_text(ui_lblHttpStatus, "");
    }

    // Poll devices
    LeftKnob.read();

    // Handle ISR requests
    if (backlight_switch_changed) {
        HandleBacklightSwitch();
    }
    if (leftknob_turned != 0) {
        HandleLeftKnobRotation(leftknob_turned);
        leftknob_turned = 0;
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
        mqttClient.beginMessage(BEDROOM_UPDATE_TOPIC);
        mqttClient.print("cfg");
        mqttClient.endMessage();
        // TODO: figure out how to not spam, but retry on failure
        // if (tv_controller.FetchTvConfig(&tv_config)) {
        //     lv_roller_set_options(ui_PlaylstSelection, tv_config.playlist_options, LV_ANIM_ON);
        //     Serial.print("Selected playlist: ");
        //     Serial.println(tv_config.current_playlist);
        // } else {
        //     // retry on failure
        //     update_tv_config_requested = true;
        // }
    }

    if (change_playlist_requested) {
        change_playlist_requested = false;
        Serial.print("Changing playlist to ");
        char playlist_name[64] = "";
        lv_roller_get_selected_str(ui_PlaylstSelection, playlist_name, 64);
        Serial.println(playlist_name);
        mqttClient.beginMessage(BEDROOM_PLAYLIST_TOPIC);
        mqttClient.print(playlist_name); // the contents are discarded anyway
        mqttClient.endMessage();
        // tv_controller.ChangePlaylist(playlist_name);
    }
    if (change_lights_requested) {
        change_lights_requested = false;
        Serial.println("Submitting updated light power...");
        SetLightsBrightness();
    }

    rtc_from_ntp(); // update the RTC clock from an NTP server once per day
    alarms.tick();
    lv_timer_handler();
    delay(10);

    // call poll() regularly to allow the library to send MQTT keep alive which
    // avoids being disconnected by the broker
    mqttClient.poll();
    if (!mqttClient.connected()) {
        Serial.println("MQTT connection lost");
        mqtt_status = 0;
    }
}

void LeftKnobRotationCallback(long new_pos, int direction) {
    leftknob_turned = direction;
}

void HandleLeftKnobRotation(const int direction) {
    chirp();
    char key_dir = direction > 0 ? LV_KEY_UP : LV_KEY_DOWN;
    // Switch tabs when the left knob is turned.
    if (leftknob_clicked) { // Tab switching mode
        auto idx = lv_tabview_get_tab_act(ui_TabView1);
        if (key_dir == LV_KEY_UP && idx < 2) {
            idx++;
        } else if (key_dir == LV_KEY_DOWN && idx > 0) {
            idx--;
        }

        lv_tabview_set_act(ui_TabView1, idx, LV_ANIM_ON);
    } else {
        switch(lv_tabview_get_tab_act(ui_TabView1)) {
        case 0:
            Serial.println("Tab 0: Clock");
            break;
        case 1:
            Serial.println("Changing the channel");
            lv_event_send(ui_PlaylstSelection, LV_EVENT_KEY, &key_dir);
            break;
        case 2:
            // Add 15 minutes to the current wakeup time
            NewWakeupTime = NewWakeupTime + (15*60*direction);
            if (NewWakeupTime <= 0) {
                Serial.print("Clamping new wakeup time to 00:01. Requested=");
                Serial.println(NewWakeupTime);
                NewWakeupTime = 60;
            } else if (NewWakeupTime > ((23 * 3600) + (45 * 60))) {
                Serial.print("Clamping new wakeup time to 23:45. Requested=");
                Serial.println(NewWakeupTime);
                NewWakeupTime = ((23 * 3600) + (45 * 60));
            }
            Serial.print("New wakeup time computed. Old=");
            Serial.print(WakeupTime);
            Serial.print(", new=");
            Serial.println(NewWakeupTime);
            // Compute the hour:minute from the new time
            const auto hour = NewWakeupTime / 3600;
            const auto minute = (NewWakeupTime % 3600) / 60;
            Serial.print("Updating UI. Time=");
            Serial.print(hour);
            Serial.print(":");
            Serial.println(minute);
            // Update the UI
            lv_label_set_text_fmt(ui_NewWakeupTime, "New Wakeup Time: %02d:%02d", hour, minute);
            break;
        }
    }
}
void HandleClickInput() {
    // Clicking the stick switches from rotating between tabs vs taking action inside the page - selecting a playlist, altering the wakeup time, etc.
    leftknob_clicked = !leftknob_clicked;
}
void DoWakeup(const tm& now) {
    Serial.println("Good morning!");
    mqttClient.beginMessage(BEDROOM_WAKEUP_TOPIC);
    mqttClient.write(1);
    mqttClient.endMessage();

    // send HTTP request to turn on the lights
    // HTTP::Response resp;
    // lights_client.exec(wakeup_request, resp);
    // if (resp.code != 204) {
    //     Serial.print("Error starting wakeup: ");
    //     Serial.print(resp.code);
    //     Serial.print(" ");
    //     Serial.println(resp.status);
    // }
}
void DoLightsOut(const tm& now) {
    Serial.println("Lights Out!");
    mqttClient.beginMessage(BEDROOM_DIMMER_TOPIC);
    mqttClient.write(0);
    mqttClient.endMessage();

    // // send HTTP request to turn off the lights
    // HTTP::Response resp;
    // lights_client.exec(lightsout_request, resp);
    // if (resp.code != 204) {
    //     Serial.print("Error ending wakeup: ");
    //     Serial.print(resp.code);
    //     Serial.print(" ");
    //     Serial.println(resp.status);
    // }
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
    const auto err = mqttClient.endMessage();
    if (err != 1) {
        Serial.print("Failed to send mqtt message on ");
        Serial.print(BEDROOM_DIMMER_TOPIC);
        Serial.print(": ");
        Serial.println(err);
        mqtt_status = 0;
    } else {
        Serial.print("MQTT message sent on ");
        Serial.println(BEDROOM_DIMMER_TOPIC);
    }
}

void chirp() {
    // analogWrite(BUZZER_PIN, 125);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(5);
    digitalWrite(BUZZER_PIN, LOW);
}

void handle_net_error(HTTP::Response *resp) {
    ConnStatus = 0; // trigger a wifi reconnect
    lv_label_set_text_fmt(ui_lblWiFiStatus, "%d", ConnStatus);
}

void handle_http_error(HTTP::Response *resp) {
    if (resp == nullptr) return;
    http_error_time = millis();
    lv_label_set_text_fmt(ui_lblHttpStatus, "%d %s", resp->code, resp->status);
}

void handleNowPlayingMessage(const String& msg) {
    UpdateNowPlaying(msg.c_str());
}
void handleMqttMessage(int messageSize) {
    const String topic = mqttClient.messageTopic();
    const String msg = mqttClient.readString();
    // Serial.print(topic);
    // Serial.print(": \"");
    // Serial.print(msg);
    // Serial.println("\"");
    lv_label_set_text(ui_lblHttpStatus, topic.c_str());
    if (topic.equals(BEDROOM_NOWPLAYING_TOPIC)) {
        handleNowPlayingMessage(msg);
    } else if (topic.equals(BEDROOM_CONFIG_TOPIC)) {
        lv_label_set_text(ui_lblHttpStatus, BEDROOM_CONFIG_TOPIC);
        HandleTvConfig(msg);
    }
    // add handlers for any new topics here
}
