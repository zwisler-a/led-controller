//
// Created by Alex on 20.10.2024.
//

#include "SetColorRequest.h"
#include "ArduinoJson.h"
#include "led.h"

std::optional<SetColorRequest> SetColorRequest::fromJson(const JsonDocument &jsonObject) {

    if (jsonObject["r"].isNull() || jsonObject["g"].isNull() || jsonObject["b"].isNull() ||
        jsonObject["time"].isNull() || jsonObject["brightness"].isNull()) {
        Serial.println("Error: Missing one or more required JSON fields.");
        return std::nullopt;
    }

    SetColorRequest request{};
    request.red = jsonObject["r"] | 0;
    request.green = jsonObject["g"] | 0;
    request.blue = jsonObject["b"] | 0;
    request.time = jsonObject["time"] | 1000;
    request.brightness = jsonObject["brightness"] | 0;
    return request;
}

ColorTransition SetColorRequest::toColorTransition() {
    return {
            red,
            green,
            blue,
            brightness,
            time
    };
}
