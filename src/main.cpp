#include <ESP8266WiFi.h>
#include "network.h"
#include "config.h"
#include "led.h"


void setup() {
    pinMode(3, FUNCTION_3);
//    Serial.begin(9600);
    config::init();
    // startConfigurationWifi LED PWM resolution/range and set pins to LOW
    analogWriteRange(256);

    network::init();
}

void loop(){
   network::update();
   led::update();
}