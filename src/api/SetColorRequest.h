//
// Created by Alex on 20.10.2024.
//

#ifndef LEDSTRIP_SETCOLORREQUEST_H
#define LEDSTRIP_SETCOLORREQUEST_H


#include <WString.h>
#include <optional>
#include "ArduinoJson.h"
#include "../led.h"

class SetColorRequest {
public:
    int red;
    int blue;
    int green;
    int brightness;
    int time;

    static std::optional<SetColorRequest> fromJson(const ArduinoJson::V720PB22::JsonDocument &jsonObject);

    ColorTransition toColorTransition();
};


#endif //LEDSTRIP_SETCOLORREQUEST_H
