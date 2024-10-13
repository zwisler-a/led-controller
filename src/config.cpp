//
// Created by Alex on 12.10.2024.
//

#include "config.h"
#include <FS.h>
#include <LittleFS.h>

String readFile(const char *path) {
    Serial.printf("Reading file: %s\n", path);

    File file = LittleFS.open(path, "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return "";
    }

    Serial.print("Read from file: ");
    if (file.available()) {
        String str = file.readString();
        Serial.println(str);
        return str;
    };
    file.close();
    return "";
}

void writeFile(const char *path, const String& message) {
    Serial.printf("Writing file: %s\n", path);

    File file = LittleFS.open(path, "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message)) {
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    delay(2000);  // Make sure the CREATE and LASTWRITE times are different
    file.close();
}

void deleteFile(const char *path) {
    Serial.printf("Deleting file: %s\n", path);
    if (LittleFS.remove(path)) {
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void config::init() {
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }
}

bool config::hasWifiConfig() {
    return LittleFS.exists("/wifi_ssid.txt") && LittleFS.exists("/wifi_pw.txt");
}

String config::getSsid() {
    return readFile("/wifi_ssid.txt");
}

void config::setSsid(const String& ssid) {
    writeFile("/wifi_ssid.txt", ssid);
}

String config::getPassword() {
    return readFile("/wifi_pw.txt");
}

void config::setPassword(const String& pw) {
    writeFile("/wifi_pw.txt", pw);
}

String config::getHostname() {
    return readFile("/wifi_name.txt");
}

void config::setHostname(const String &name) {
    writeFile("/wifi_name.txt", name);
}

void config::clearWifiConfig() {
    deleteFile("/wifi_ssid.txt");
    deleteFile("/wifi_name.txt");
    deleteFile("/wifi_pw.txt");
}
