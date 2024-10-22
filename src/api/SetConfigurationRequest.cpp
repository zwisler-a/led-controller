//
// Created by Alex on 20.10.2024.
//

#include "SetConfigurationRequest.h"
#include "ArduinoJson.h"

std::optional<SetConfigurationRequest> SetConfigurationRequest::fromJson(const JsonDocument &jsonObject) {

    if (jsonObject["ssid"].isNull() || jsonObject["password"].isNull() || jsonObject["name"].isNull() ||
        jsonObject["userId"].isNull() || jsonObject["mqttServer"].isNull()|| jsonObject["mqttUser"].isNull()|| jsonObject["mqttPass"].isNull()) {
        Serial.println("Error: Missing one or more required JSON fields.");
        return std::nullopt;
    }

    SetConfigurationRequest request;
    request.ssid = jsonObject["ssid"] | "";
    request.password = jsonObject["password"] | "";
    request.name = jsonObject["name"] | "";
    request.userId = jsonObject["userId"] | "";
    request.mqttServer = jsonObject["mqttServer"] | "";
    request.mqttUser = jsonObject["mqttUser"] | "";
    request.mqttPass = jsonObject["mqttPass"] | "";
    return request;
}

