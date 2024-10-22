//
// Created by Alex on 20.10.2024.
//

#ifndef LEDSTRIP_SETCONFIGURATIONREQUEST_H
#define LEDSTRIP_SETCONFIGURATIONREQUEST_H


#include <WString.h>
#include <optional>
#include <ArduinoJson.h>

class SetConfigurationRequest {
public:
    String ssid;
    String password;
    String name;
    String userId;
    String mqttServer;
    String mqttUser;
    String mqttPass;

    static std::optional<SetConfigurationRequest> fromJson(const JsonDocument &json);
};


#endif //LEDSTRIP_SETCONFIGURATIONREQUEST_H
