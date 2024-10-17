//
// Created by Alex on 12.10.2024.
//

#ifndef LEDSTRIP_CONFIG_H
#define LEDSTRIP_CONFIG_H


#include <string>
#include <WString.h>
#include <ArduinoJson.h>

class config {
public:
    static void init();
    static bool hasWifiConfig();
    static String getSsid();
    static String getPassword();
    static String getHostname();
    static void clearWifiConfig();

    static void setWifiConfig(const String &ssid, const String &password, const String &name);
};


#endif //LEDSTRIP_CONFIG_H
