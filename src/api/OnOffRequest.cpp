//
// Created by Alex on 21.10.2024.
//

#include "OnOffRequest.h"

std::optional<OnOffRequest> OnOffRequest::fromJson(JsonDocument jsonObject) {
    OnOffRequest onOffRequest{};
    onOffRequest.on = jsonObject["on"];
    return onOffRequest;
}
