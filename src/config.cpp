//
// Created by Alex on 12.10.2024.
//

#include "config.h"
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

String readFile(const char *path) {
    Serial.printf("Reading file");

    File file = LittleFS.open(path, "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return "{}";
    }

    if (file.available()) {
        String str = file.readString();
        return str;
    };
    file.close();
    return "{}";
}

void deleteFile(const char *path) {
    Serial.printf("Deleting file: %s\n", path);
    if (LittleFS.remove(path)) {
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

JsonDocument configDoc;

void config::init() {
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }
    deserializeJson(configDoc, readFile("/config.json"));
}

bool config::hasWifiConfig() {
    return configDoc["ssid"] != nullptr && configDoc["password"] != nullptr && configDoc["name"] != nullptr;
}

String config::getSsid() {
    return configDoc["ssid"];
}

String config::getPassword() {
    return configDoc["password"];
}

String config::getHostname() {
    return configDoc["name"];
}

void config::setWifiConfig(const String &ssid, const String &password, const String &name) {
    configDoc["ssid"] = ssid;
    configDoc["password"] = password;
    configDoc["name"] = name;
    File file = LittleFS.open("/config.json", "w");
    if (!file) {
        Serial.println("Could not open config file to write");
        return;
    }
    serializeJson(configDoc, file);
}

void config::clearWifiConfig() {
    deleteFile("/config.json");
}
