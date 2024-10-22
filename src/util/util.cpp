//
// Created by Alex on 20.10.2024.
//

#include <cstdint>
#include <algorithm>
#include "util.h"

int32_t util::rgbToSpectrumRGB(int red, int green, int blue) {
    // Ensure the RGB values are clamped between 0 and 255
    red = std::max(0, std::min(255, red));
    green = std::max(0, std::min(255, green));
    blue = std::max(0, std::min(255, blue));

    // Combine the RGB values into a single 24-bit integer
    return (red << 16) | (green << 8) | blue;
}

JsonDocument util::parseJson(const String &json) {
    JsonDocument jsonObject;
    DeserializationError err = deserializeJson(jsonObject, json);
    if (err) {
        Serial.println("Error while parsing json");
    }
    return jsonObject;
}

String util::serializeJson(const JsonDocument &json) {
    String string;
    ArduinoJson::serializeJson(json, string);
    return string;

}
