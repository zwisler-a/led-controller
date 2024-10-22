//
// Created by Alex on 12.10.2024.
//

#include "led.h"
#include "Arduino.h"
#include "network/mqtt.h"

struct Color {
    int red;
    int green;
    int blue;
    int brightness;
};

Color currentColor{255, 255, 255, 100};
bool isOn = true;
int brightnessMemory;
Color transitionColor{};

unsigned long startTime = 0;

int duration;

float stepRed;
float stepGreen;
float stepBlue;
float stepBrightness;
bool inTransition = false;


bool inAnimation = false;
bool repeatAnimation = false;
std::vector<ColorTransition> animation;
int currentAnimationTransition = 0;


const int redPin = 0;
const int greenPin = 3;
const int bluePin = 2;

void transitionToColor(ColorTransition colorTransition) {
    if (inTransition) return;
    isOn = true;
    int steps = 100;
    startTime = millis();
    duration = colorTransition.duration;
    stepRed = (float) (colorTransition.red - currentColor.red) / steps;
    stepGreen = (float) (colorTransition.green - currentColor.green) / steps;
    stepBlue = (float) (colorTransition.blue - currentColor.blue) / steps;
    stepBrightness = (float) (colorTransition.brightness - currentColor.brightness) / steps;
    inTransition = true;
    transitionColor = currentColor;
    currentColor = {
            colorTransition.red,
            colorTransition.green,
            colorTransition.blue,
            colorTransition.brightness
    };
}

void led::setColor(ColorTransition colorTransition) {
    inAnimation = false;
    if (inTransition) return;
    transitionToColor(colorTransition);
}

int gammaCorrection(int value) {
    return std::pow((float)value / 255.0, 2.2) * 255.0;
}

void led::update() {
    if (!inTransition) return;

    unsigned long elapsedTime = millis() - startTime;
    float progress = (float) elapsedTime / duration; // Calculate the progress as a fraction

    // Ensure progress is between 0 and 1
    if (progress > 1.0) {
        progress = 1.0;
        inTransition = false;
    }
    if (progress < 0.0) progress = 0.0;
    // Calculate the new color values based on progress
    int newBrightness = transitionColor.brightness + (int) (stepBrightness * progress * 100);
    int newRed = transitionColor.red + (int) (stepRed * progress * 100);
    int newGreen = transitionColor.green + (int) (stepGreen * progress * 100);
    int newBlue = transitionColor.blue + (int) (stepBlue * progress * 100);
    Serial.printf("COLOR: %d %d %d %d \n", newRed, newGreen, newBlue, newBrightness);
    analogWrite(redPin, gammaCorrection((newRed * newBrightness) / 100));
    analogWrite(greenPin, gammaCorrection((newGreen * newBrightness) / 100));
    analogWrite(bluePin, gammaCorrection((newBlue * newBrightness) / 100));

    if (!inTransition) {
        if (inAnimation) {
            if (currentAnimationTransition < animation.size() - 1) {
                currentAnimationTransition++;
                transitionToColor(animation[currentAnimationTransition]);
            } else if (repeatAnimation) {
                Serial.println("Repeating animation");
                currentAnimationTransition = 0;
                transitionToColor(animation[currentAnimationTransition]);
            } else {
                inAnimation = false;
                Serial.println("Stopping animation");
                mqtt::publishState();
            }
        } else {
            if(isOn) {
                mqtt::publishState();
            }
        }
    }
}

void led::setColorAnimation(const std::vector<ColorTransition> &transitions, bool repeating) {
    animation = transitions;
    repeatAnimation = repeating;
    inAnimation = true;
    currentAnimationTransition = 0;
    transitionToColor(animation[currentAnimationTransition]);
}

ColorTransition led::getColor() {
    return {
            currentColor.red,
            currentColor.green,
            currentColor.blue,
            currentColor.brightness,
            0
    };
}

void led::setOn(bool on) {
    if (!on) {
        brightnessMemory = currentColor.brightness;
        transitionToColor({
                                  currentColor.red,
                                  currentColor.green,
                                  currentColor.blue,
                                  0,
                                  1000
                          });
    } else {
        transitionToColor({
                                  currentColor.red,
                                  currentColor.green,
                                  currentColor.blue,
                                  brightnessMemory,
                                  1000
                          });
    }
    isOn = on;
}

bool led::getOn() {
    return isOn;
}

