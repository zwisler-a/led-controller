//
// Created by Alex on 12.10.2024.
//

#ifndef LEDSTRIP_CONFIG_H
#define LEDSTRIP_CONFIG_H


#include <string>
#include <WString.h>

class config {
public:
    static void init();
    static bool hasWifiConfig();
    static String getSsid();
    static void setSsid(const String& ssid);
    static String getPassword();
    static void setPassword(const String& pw);
    static String getHostname();
    static void setHostname(const String& name);
    static void clearWifiConfig();

};


#endif //LEDSTRIP_CONFIG_H
