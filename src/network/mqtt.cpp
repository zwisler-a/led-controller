//
// Created by Alex on 18.10.2024.
//

#include <ESP8266WiFi.h>
#include "mqtt.h"
#include "../config.h"
#include "Arduino.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include "led.h"
#include "util/util.h"
#include "api/SetColorRequest.h"
#include "api/SetAnimationRequest.h"
#include "api/UpdateStatusResponse.h"
#include "api/OnOffRequest.h"

WiFiClientSecure espClient;
// WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    // Create a buffer to store the payload
    char jsonBuffer[length + 1];
    memcpy(jsonBuffer, payload, length);
    jsonBuffer[length] = '\0';  // Null-terminate the string

    JsonDocument jsonObject = util::parseJson(jsonBuffer);


    if (jsonObject["type"] == "set_color") {
        std::optional<SetColorRequest> maybeColorRequest = SetColorRequest::fromJson(jsonObject);
        if (maybeColorRequest.has_value()) {
            led::setColor(maybeColorRequest.value().toColorTransition());
        }
    }
    if (jsonObject["type"] == "set_anim") {
        std::optional<SetAnimationRequest> maybeAnimationRequest = SetAnimationRequest::fromJson(jsonObject);
        if (maybeAnimationRequest.has_value()) {
            led::setColorAnimation(maybeAnimationRequest->transitions, maybeAnimationRequest->repeating);
        }
    }
    if (jsonObject["type"] == "on_off") {
        std::optional<OnOffRequest> maybeOnOff = OnOffRequest::fromJson(jsonObject);
        if (maybeOnOff.has_value()) {
            led::setOn(maybeOnOff->on);
        }
    }


    serializeJson(jsonObject, Serial);
    Serial.println();
}

void reconnect() {
    while (!client.connected()) {
        Serial.printf("Attempting MQTT connection to \"%s\"...", config::configuration.mqttServer.c_str());
        String clientId = config::configuration.name;
        if (client.connect(clientId.c_str(), config::configuration.mqttUser.c_str(),
                           config::configuration.mqttPass.c_str())) {
            Serial.println("connected");
            Serial.println("Sending announcement.");
            String announcement = config::getAnnouncement();
            Serial.println(announcement);
            client.publish("/ada/discover", announcement.c_str());
            String topic = "/ada/" + config::configuration.userId + "/" + String(config::getDeviceId()) + "/action";
            client.subscribe(topic.c_str());
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqtt::publishState() {
    String topic = "/ada/" + config::configuration.userId + "/" + String(config::getDeviceId()) + "/status";
    client.publish(topic.c_str(),
                   UpdateStatusResponse::fromColorTransition(led::getColor(), led::getOn()).toJson().c_str());
}

void mqtt::begin() {
    client.setServer(config::configuration.mqttServer.c_str(), 1883);
    client.setCallback(callback);
    espClient.setInsecure();
    client.setBufferSize(1024);
}

void mqtt::update() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
