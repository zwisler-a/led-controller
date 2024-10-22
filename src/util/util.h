//
// Created by Alex on 20.10.2024.
//

#ifndef LEDSTRIP_UTIL_H
#define LEDSTRIP_UTIL_H
#include "ArduinoJson.h"

class util {
public:
    static int32_t rgbToSpectrumRGB(int red, int green, int blue);
    static JsonDocument parseJson(const String& json);

    static String serializeJson(const JsonDocument &json);
};


#endif //LEDSTRIP_UTIL_H
