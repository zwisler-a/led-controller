//
// Created by Alex on 12.10.2024.
//

#ifndef LEDSTRIP_LED_H
#define LEDSTRIP_LED_H


#include <vector>

struct ColorTransition {
    int red;
    int green;
    int blue;
    int brightness;
    int duration;
};

class led {
public:
    static void update();
    static void setColor(ColorTransition colorTransition);
    static void setColorAnimation(const std::vector<ColorTransition>& transitions, bool repeating);
    static ColorTransition getColor();

    static void setOn(bool on);
    static bool getOn();
};


#endif //LEDSTRIP_LED_H
