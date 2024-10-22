//
// Created by Alex on 12.10.2024.
//

#include "network.h"
#include "../config.h"
#include "led.h"
#include <ESP8266WiFi.h>
#include "webconfig.h"
#include "web.h"
#include "mqtt.h"


const std::vector<ColorTransition> connectedAnimation = {
        {0, 255, 0, 100, 500},
        {0, 255,   0, 100, 500},
        {255, 255,   255, 0, 10}
};


void connectWifi() {
    unsigned long start = millis();
    WiFi.hostname(config::configuration.name);
    WiFi.begin(config::configuration.ssid, config::configuration.password);
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
        delay(500);
    }
    if (WiFi.status() != WL_CONNECTED) {
        config::clearConfig();
//        webconfig::startConfigurationWifi();
        Serial.println("Failed to connect to Wifi ... reconfiguring");
    }
    led::setColorAnimation(connectedAnimation, false);
}

void network::init() {
    if (!config::isConfigured()) {
        webconfig::startConfigurationWifi();
    } else {
        connectWifi();
    }
    web::begin();
    mqtt::begin();
}


void network::update() {
    webconfig::update();
    web::update();
    mqtt::update();
}

