//
// Created by Alex on 21.10.2024.
//

#ifndef LEDSTRIP_ONOFFREQUEST_H
#define LEDSTRIP_ONOFFREQUEST_H


#include <optional>
#include "ArduinoJson.h"

class OnOffRequest {
public:
    bool on;

    static std::optional<OnOffRequest> fromJson(JsonDocument jsonObject);
};


#endif //LEDSTRIP_ONOFFREQUEST_H
