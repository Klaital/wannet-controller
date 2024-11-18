//
// Created by Kit on 8/26/2024.
//

#ifndef TVCONTROLCLIENT_H
#define TVCONTROLCLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <MqttClient.h>

#include "TvConfig.h"

class TvControlClient {
    HttpClient net;

public:
    TvControlClient(const char* hostname, const int port, WiFiClient* net) {
        strcpy(this->net.host, hostname);
        this->net.port = port;
        this->net.net = net;
    }

    void RegisterHandlers(const HttpCallback net_error, const HttpCallback display_resp) {
        this->net.net_error_callback = net_error;
        this->net.resp_callback = display_resp;
    }

    int FetchTvConfig(TvConfig* cfg);
    int ChangePlaylist(const char* playlist);
};



#endif //TVCONTROLCLIENT_H
