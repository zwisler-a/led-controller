//
// Created by Alex on 20.10.2024.
//

#include "SetAnimationRequest.h"

std::optional<SetAnimationRequest> SetAnimationRequest::fromJson(const JsonDocument &jsonObject) {

    std::vector<ColorTransition> transitions;
    bool repeating = jsonObject["repeating"] | false;

    if (jsonObject["transitions"].is<JsonArray>()) {
        JsonArrayConst transitionsArray = jsonObject["transitions"];
        for (JsonObjectConst transition: transitionsArray) {
            if (transition["r"].is<int>() && transition["g"].is<int>() &&
                transition["b"].is<int>() && transition["time"].is<int>()) {
                ColorTransition colorTransition = {
                        transition["r"],
                        transition["g"],
                        transition["b"],
                        jsonObject["brightness"],
                        transition["time"]
                };
                transitions.push_back(colorTransition);
            }
        }
    }

    SetAnimationRequest animationRequest;
    animationRequest.transitions = transitions;
    animationRequest.repeating = repeating;
    return animationRequest;
}
