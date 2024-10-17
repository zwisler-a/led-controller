//
// Created by Alex on 12.10.2024.
//

#include "led.h"
#include "Arduino.h"

int currentRed = 0;
int currentGreen = 0;
int currentBlue = 0;

unsigned long startTime = 0;

int duration;
float stepRed;
float stepGreen;
float stepBlue;
bool inTransition = false;


bool inAnimation = false;
bool repeatAnimation = false;
std::vector<ColorTransition> animation;
int currentAnimationTransition = 0;


const int redPin = 0;
const int greenPin = 3;
const int bluePin = 2;

void transitionToColor(ColorTransition colorTransition) {
    Serial.print("Transitioning to:");
    Serial.print(colorTransition.red);
    Serial.print(", ");
    Serial.print(colorTransition.green);
    Serial.print(", ");
    Serial.print(colorTransition.blue);
    Serial.print(" | ");
    Serial.println(colorTransition.duration);
    int steps = 100;
    startTime = millis();
    duration = colorTransition.duration;
    stepRed = (float) (colorTransition.red - currentRed) / steps;
    stepGreen = (float) (colorTransition.green - currentGreen) / steps;
    stepBlue = (float) (colorTransition.blue - currentBlue) / steps;
    inTransition = true;
}

void led::setColor(ColorTransition colorTransition) {
    inAnimation = false;
    transitionToColor(colorTransition);
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
    int newRed = currentRed + (int) (stepRed * progress * 100);
    int newGreen = currentGreen + (int) (stepGreen * progress * 100);
    int newBlue = currentBlue + (int) (stepBlue * progress * 100);

    analogWrite(redPin, newRed);
    analogWrite(greenPin, newGreen);
    analogWrite(bluePin, newBlue);

    if (!inTransition) {
        currentBlue = newBlue;
        currentRed = newRed;
        currentGreen = newGreen;
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
            currentRed,
            currentGreen,
            currentBlue,
            0
    };
}

