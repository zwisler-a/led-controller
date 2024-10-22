//
// Created by Alex on 18.10.2024.
//

#include "network.h"
#include "led.h"
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "ArduinoJson.h"

#include "webconfig.h"
#include "config.h"
#include "api/SetConfigurationRequest.h"
#include "util/util.h"


ESP8266WebServer configServer(80);
const char *apSsid = "ESP8266-Configuration-AP";
const char *apPassword = "123456789";
bool configIsRunning = false;
const std::vector<ColorTransition> configurationAnimation = {
        {255, 0, 0, 100, 500},
        {0,   0, 0, 100, 500},
        {126, 0, 0, 100, 500},
        {0,   0, 0, 100, 500}
};

void handleConfigSetup() {
    std::optional<SetConfigurationRequest> maybeRequest = SetConfigurationRequest::fromJson(
            util::parseJson(configServer.arg("plain")));
    if (!maybeRequest.has_value()) {
        configServer.send(400, R"({"success": false})");;
        return;
    }
    config::setConfiguration(maybeRequest.value());
    Serial.printf("Updated Configuration");
    configServer.send(200, R"({"success": true})");
    webconfig::stopConfigurationWifi();
}

void handleWebConfigNotFound() {
    configServer.send(404, "application/json", R"({"error":"not found"})");
}

void setupWebConfigRoutes() {
    configServer.serveStatic("/config", LittleFS, "/config.html");
    configServer.on("/config", HTTP_POST, handleConfigSetup);
    configServer.onNotFound(handleWebConfigNotFound);
}

void webconfig::startConfigurationWifi() {
    WiFi.softAP(apSsid, apPassword);
    led::setColorAnimation(configurationAnimation, true);
    configServer.begin();
    setupWebConfigRoutes();
    configIsRunning = true;
}

void webconfig::stopConfigurationWifi() {
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
    configServer.close();
    configIsRunning = false;
}

void webconfig::update() {
    if (configIsRunning) {
        configServer.handleClient();
    }
}