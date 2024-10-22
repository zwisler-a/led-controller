//
// Created by Alex on 20.10.2024.
//

#include "UpdateStatusResponse.h"
#include "ArduinoJson.h"
#include "util/util.h"
#include "led.h"

String UpdateStatusResponse::toJson() const {
    JsonDocument doc;
    doc["on"] = on;
    doc["brightness"] = brightness;
    JsonObject colorObject = doc.createNestedObject("color");
    colorObject["spectrumRGB"] = spectrumRGB;
    return util::serializeJson(doc);
}

UpdateStatusResponse UpdateStatusResponse::fromColorTransition(ColorTransition transition, bool onState) {
    return UpdateStatusResponse{
            onState,
            transition.brightness,
            util::rgbToSpectrumRGB(transition.red, transition.green, transition.blue)
    };
}
