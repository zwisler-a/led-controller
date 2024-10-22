//
// Created by Alex on 18.10.2024.
//

#ifndef LEDSTRIP_MQTT_H
#define LEDSTRIP_MQTT_H


class mqtt {
public:
    static void update();

    static void begin();

    static void publishState();
};


#endif //LEDSTRIP_MQTT_H
