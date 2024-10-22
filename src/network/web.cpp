//
// Created by Alex on 18.10.2024.
//

#include "led.h"
#include "PubSubClient.h"
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "web.h"
#include "api/SetColorRequest.h"
#include "util/util.h"
#include "api/SetAnimationRequest.h"
#include "api/UpdateStatusResponse.h"


ESP8266WebServer server(80);

void handleGetStatus() {
    server.send(200, "application/json",
                UpdateStatusResponse::fromColorTransition(led::getColor(), led::getOn()).toJson());
}

void handleSetColor() {
    std::optional<SetColorRequest> maybeColorRequest = SetColorRequest::fromJson(util::parseJson(server.arg("plain")));
    if (maybeColorRequest.has_value()) {
        led::setColor(maybeColorRequest.value().toColorTransition());
        server.send(200, R"({"success": true})");
    } else {
        server.send(400, R"({"success": false})");
    }
}

void handleSetAnimation() {
    std::optional<SetAnimationRequest> maybeAnimationRequest = SetAnimationRequest::fromJson(
            util::parseJson(server.arg("plain")));
    if (maybeAnimationRequest.has_value()) {
        led::setColorAnimation(maybeAnimationRequest->transitions, maybeAnimationRequest->repeating);
        server.send(200, "application/json", R"({"success": true})");
    } else {
        server.send(400, "application/json", R"({"success": false, "error": "No transitions provided"})");
    }
}

void handleNotFound() {
    server.send(404, "application/json", R"({"error":"not found"})");
}

void setupRoutes() {
    server.serveStatic("/", LittleFS, "/index.html");
    server.serveStatic("/coloris.min.js", LittleFS, "/coloris.min.js");
    server.serveStatic("/coloris.min.css", LittleFS, "/coloris.min.css");

    server.on("/set", HTTP_POST, handleSetColor);
    server.on("/animate", HTTP_POST, handleSetAnimation);
    server.on("/status", HTTP_GET, handleGetStatus);
    server.onNotFound(handleNotFound);
}

void web::begin() {
    server.begin();
    setupRoutes();
}

void web::update() {
    server.handleClient();
}