//
// Created by Alex on 20.10.2024.
//

#ifndef LEDSTRIP_SETANIMATIONREQUEST_H
#define LEDSTRIP_SETANIMATIONREQUEST_H


#include "led.h"
#include "ArduinoJson.h"

class SetAnimationRequest {
public:
    bool repeating;
    std::vector<ColorTransition> transitions;

    static std::optional<SetAnimationRequest> fromJson(const JsonDocument& jsonObject);
};


#endif //LEDSTRIP_SETANIMATIONREQUEST_H
