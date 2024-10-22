//
// Created by Alex on 12.10.2024.
//

#include "config.h"
#include "util/util.h"
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

Configuration config::configuration;

String readFile(const char *path) {
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


void config::init() {
    JsonDocument configDoc;
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }
    deserializeJson(configDoc, readFile("/config.json"));
    config::configuration = {
            configDoc["ssid"],
            configDoc["password"],
            configDoc["name"],
            configDoc["userId"],
            configDoc["mqttServer"],
            configDoc["mqttUser"],
            configDoc["mqttPass"]
    };
}

bool config::isConfigured() {
    return !config::configuration.ssid.isEmpty() &&
           !config::configuration.password.isEmpty() &&
           !config::configuration.name.isEmpty() &&
           !config::configuration.userId.isEmpty() &&
           !config::configuration.mqttServer.isEmpty();
}

void config::setConfiguration(const SetConfigurationRequest &request) {
    JsonDocument configDoc;
    configDoc["ssid"] = request.ssid;
    configDoc["password"] = request.password;
    configDoc["name"] = request.name;
    configDoc["userId"] = request.userId;
    configDoc["mqttServer"] = request.mqttServer;
    configDoc["mqttUser"] = request.mqttUser;
    configDoc["mqttPass"] = request.mqttPass;
    File file = LittleFS.open("/config.json", "w");
    if (!file) {
        Serial.println("Could not open config file to write");
        return;
    }
    serializeJson(configDoc, file);
}


void config::clearConfig() {
    deleteFile("/config.json");
}

String config::getAnnouncement() {
    JsonDocument deviceInfo = util::parseJson(readFile("/device.json"));
    deviceInfo["id"] = getDeviceId();
    deviceInfo["name"]["name"] = configuration.name;
    JsonDocument announcement;
    announcement["device"] = deviceInfo;
    announcement["userId"] = configuration.userId;
    announcement["deviceId"] = getDeviceId();
    return util::serializeJson(announcement);
}

String config::getDeviceId() {
    return String(EspClass::getChipId());
}


