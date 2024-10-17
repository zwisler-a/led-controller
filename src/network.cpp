//
// Created by Alex on 12.10.2024.
//

#include "network.h"
#include "config.h"
#include "led.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

ESP8266WebServer server(80);

const char *apSsid = "ESP8266-Configuration-AP";
const char *apPassword = "123456789";
const char *CONTENT_TYPE = "application/json";
const std::vector<ColorTransition> configurationAnimation = {
        {255, 0, 0, 500},
        {0, 0, 0, 500},
        {126, 0, 0, 500},
        {0, 0, 0, 500},
};

const std::vector<ColorTransition> connectedAnimation = {
        {0, 255, 0, 500},
        {0, 0, 0,   500}
};

void startConfigurationWifi() {
    WiFi.softAP(apSsid, apPassword);
    if (MDNS.begin("esp")) Serial.println("MDNS responder started");
    led::setColorAnimation(configurationAnimation, true);
}

void stopConfigurationWifi() {
    MDNS.close();
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
}

void handleNotFound() {
    server.send(404, CONTENT_TYPE, R"({"error":"not found"})");
}

void handleGetStatus() {
    JsonDocument response;
    ColorTransition current = led::getColor();
    response["r"] = current.red;
    response["g"] = current.green;
    response["b"] = current.blue;
    String string;
    serializeJson(response, string);
    server.send(200, CONTENT_TYPE, string);
}

void handleConfigSetup() {
    JsonDocument body;
    deserializeJson(body, server.arg("plain"));
    Serial.println("Got configuration request.");
    if (body["ssid"] != nullptr && body["password"] != nullptr && body["name"] != nullptr) {
        config::setWifiConfig(
                body["ssid"],
                body["password"],
                body["name"]
        );
        server.send(200, R"({"success": true})");
        stopConfigurationWifi();
        init();
    } else {
        server.send(400, R"({"success": false})");
    }
}

void handleSetColor() {
    JsonDocument body;
    deserializeJson(body, server.arg("plain"));
    Serial.println("Got color request.");
    if (body["r"] != nullptr && body["g"] != nullptr && body["b"] != nullptr) {
        led::setColor({
                              body["r"],
                              body["g"],
                              body["b"],
                              body["time"]
                      });
        server.send(200, R"({"success": true})");
    } else {
        server.send(400, R"({"success": false})");
    }
}

void handleSetAnimation() {
    JsonDocument body;
    DeserializationError err = deserializeJson(body, server.arg("plain"));
    if (err) {
        server.send(400, "application/json", R"({"success": false, "error": "Invalid JSON format"})");
        return;
    }

    std::vector<ColorTransition> transitions;
    bool repeating = body["repeating"] | false;

    if (body["transitions"].is<JsonArray>()) {
        for (JsonObject transition: body["transitions"].as<JsonArray>()) {
            if (transition["r"].is<int>() && transition["g"].is<int>() &&
                transition["b"].is<int>() && transition["time"].is<int>()) {
                ColorTransition colorTransition = {
                        transition["r"],
                        transition["g"],
                        transition["b"],
                        transition["time"]
                };
                transitions.push_back(colorTransition);
            } else {
                server.send(400, "application/json", R"({"success": false, "error": "Invalid transition format"})");
                return;
            }
        }

        led::setColorAnimation(transitions, repeating);
        server.send(200, "application/json", R"({"success": true})");
    } else {
        server.send(400, "application/json", R"({"success": false, "error": "No transitions provided"})");
    }
}

void setupRoutes() {
    server.serveStatic("/config", LittleFS, "/config.html");
    server.serveStatic("/", LittleFS, "/index.html");
    server.serveStatic("/coloris.min.js", LittleFS, "/coloris.min.js");
    server.serveStatic("/coloris.min.css", LittleFS, "/coloris.min.css");

    server.on("/config", HTTP_POST, handleConfigSetup);
    server.on("/set", HTTP_POST, handleSetColor);
    server.on("/animate", HTTP_POST, handleSetAnimation);
    server.on("/status", HTTP_GET, handleGetStatus);
    server.onNotFound(handleNotFound);
}

void connectWifi() {
    unsigned long start = millis();
    WiFi.hostname(config::getHostname());
    WiFi.begin(config::getSsid(), config::getPassword());
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
        delay(500);
    }
    if (WiFi.status() != WL_CONNECTED) {
        config::clearWifiConfig();
        startConfigurationWifi();
        Serial.println("Failed to connect to Wifi ... reconfiguring");
    }
    if (MDNS.begin(config::getHostname())) Serial.println("MDNS responder started");
    led::setColorAnimation(connectedAnimation, false);
}

void network::init() {
    if (!config::hasWifiConfig()) {
        startConfigurationWifi();
    } else {
        connectWifi();
    }
    server.begin();
    setupRoutes();
}


void network::update() {
    server.handleClient();
}

