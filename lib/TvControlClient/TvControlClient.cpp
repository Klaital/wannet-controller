//
// Created by Kit on 8/26/2024.
//

#include "TvControlClient.h"
#include "Request.h"
#include "Response.h"

int TvControlClient::FetchTvConfig(TvConfig *cfg) {
    HTTP::Request req;
    strcpy(req.method,"GET");
    strcpy(req.path, "/cfg");
    req.headers.set("Accept", "application/text");
    HTTP::Response resp;
    this->net.exec(req, resp);
    if (resp.code >= 300 || resp.code == 0) {
        Serial.print("Failed to fetch TV Config: ");
        Serial.print(resp.code);
        Serial.print(" ");
        Serial.println(resp.status);
        return 0;
    }

    cfg->parse(resp.body);
    Serial.println("Successfully parsed config");
    // Serial.println(cfg->playlist_options);
    return 1;
}

int TvControlClient::ChangePlaylist(const char *playlist) {
    HTTP::Request req;
    strcpy(req.method, "GET");
    strcpy(req.path, "/cfg/playlist");
    req.headers.set("Content-Type", "application/json");
    strcpy(req.body, R"({"playlist":")");
    strcat(req.body, playlist);
    strcat(req.body, R"("})");
    HTTP::Response resp;
    net.exec(req, resp);
    if (resp.code >= 300 || resp.code == 0) {
        Serial.print("Failed to set playlist: ");
        Serial.print(resp.code);
        Serial.print(" ");
        Serial.println(resp.status);
        return 0;
    }

    // Success!
    return 1;
}
