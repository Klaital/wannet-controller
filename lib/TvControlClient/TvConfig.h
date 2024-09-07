//
// Created by Kit on 8/26/2024.
//

#ifndef TVCONFIG_H
#define TVCONFIG_H

#include <Arduino.h>
#include <keyval.h>

#define MAX_PLAYLISTS 10

class TvConfig {
public:
    char playlist_options[256] = "";
    char current_playlist[64] = "";
    bool shuffle = false;
    bool loop = false;

    TvConfig() = default;
    explicit TvConfig(char* doc) {
        parse(doc);
    }
    void parse(char* doc) {
        KeyValSet kv;
        kv.parse(doc);
        this->shuffle = kv.get_bool("shuffle");
        this->loop = kv.get_bool("loop");
        const char *selected_playlist = kv.get("selected_playlist");
        if (selected_playlist == nullptr) {
            Serial.println("No Selected Playlist in config!");
        } else {
            strcpy(this->current_playlist, selected_playlist);
            Serial.print("SelectedPlaylist=");
            Serial.println(this->current_playlist);
        }
        const char *playlist_list = kv.get("playlists_available");
        if (playlist_list == nullptr) {
            Serial.println("No playlists listed!");
        } else {
            strcpy(this->playlist_options, kv.get("playlists_available"));
            // replace the commas with newlines
            for (size_t i=0; i < strlen(this->playlist_options); i++) {
                if (this->playlist_options[i] == ',') {
                    this->playlist_options[i] = '\n';
                }
            }
        }
    }

    // Given a playlist ID from the UI scroller, return the playlist
    // name to send to the backend.
    void playlist_at_index(const size_t idx, char *playlist) const {
        char buf[256];
        strcpy(buf, this->playlist_options);
        const char *line = strtok(buf, "\n");
        size_t playlists_counted=1;
        while(line != nullptr) {
            if (idx == playlists_counted) {
                playlist[0] = '\0';
                strcpy(playlist, line);
                return;
                // return {line};
            }
            playlists_counted++;
        }
    }

    // Given an active playlist name, return the 1-indexed ID to set in the UI roller
    size_t index_of_playlist(const char* playlist) const {
        size_t newlines = 0;
        String tmp = this->playlist_options;
        auto head = tmp.indexOf(playlist);
        for (size_t i =0; i < head; i++) {
            if (this->playlist_options[i] == '\n') {
                newlines++;
            }
        }

        return newlines+1;
    }
};

#endif //TVCONFIG_H
