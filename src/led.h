//
// Created by Alex on 12.10.2024.
//

#ifndef LEDSTRIP_LED_H
#define LEDSTRIP_LED_H


class led {
public:
    static void setColorBlocking(int bTargetRed, int bTargetGreen, int bTargetBlue, int bDuration);
    static void update();
    static bool isInAnimation();
    static void setColor(int red, int green, int blue, int dur);
};


#endif //LEDSTRIP_LED_H
