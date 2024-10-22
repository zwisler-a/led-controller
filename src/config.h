//
// Created by Alex on 12.10.2024.
//

#ifndef LEDSTRIP_CONFIG_H
#define LEDSTRIP_CONFIG_H


#include <WString.h>
#include "api/SetConfigurationRequest.h"

struct Configuration {
    String ssid;
    String password;
    String name;
    String userId;
    String mqttServer;
    String mqttUser;
    String mqttPass;
};

class config {
public:
    static Configuration configuration;

    static void init();

    static bool isConfigured();

    static String getAnnouncement();

    static void clearConfig();

    static void setConfiguration(const SetConfigurationRequest &request);

    static String getDeviceId();

};


#endif //LEDSTRIP_CONFIG_H
