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
bool inAnimation = false;

const int redPin = 0;
const int greenPin = 3;
const int bluePin = 2;

void led::setColorBlocking(int bTargetRed, int bTargetGreen, int bTargetBlue, int bDuration) {
    int steps = 100; // Number of steps in the transition
    int delayTime = bDuration / steps; // Delay between each step

    // Use floats to calculate the step sizes for smoother transitions
    stepRed = (float) (bTargetRed - currentRed) / steps;
    stepGreen = (float) (bTargetGreen - currentGreen) / steps;
    stepBlue = (float) (bTargetBlue - currentBlue) / steps;

    for (int i = 0; i <= steps; i++) {
        analogWrite(redPin, currentRed + (int) (stepRed * i));
        analogWrite(greenPin, currentGreen + (int) (stepGreen * i));
        analogWrite(bluePin, currentBlue + (int) (stepBlue * i));
        delay(delayTime);
    }

    // Update current color values
    currentRed = bTargetRed;
    currentGreen = bTargetGreen;
    currentBlue = bTargetBlue;
}

void led::setColor(int red, int green, int blue, int dur) {
    if(inAnimation) return;
    int steps = 100;
    startTime = millis();
    duration = dur;
    stepRed = (float) (red - currentRed) / steps;
    stepGreen = (float) (green - currentGreen) / steps;
    stepBlue = (float) (blue - currentBlue) / steps;
    inAnimation = true;
}

void led::update() {
    if (!inAnimation) return;

    unsigned long elapsedTime = millis() - startTime;
    float progress = (float) elapsedTime / duration; // Calculate the progress as a fraction

    // Ensure progress is between 0 and 1
    if (progress > 1.0) {
        progress = 1.0;
        inAnimation = false;
    }
    if (progress < 0.0) progress = 0.0;

    // Calculate the new color values based on progress
    int newRed = currentRed + (int) (stepRed * progress * 100);
    int newGreen = currentGreen + (int) (stepGreen * progress * 100);
    int newBlue = currentBlue + (int) (stepBlue * progress * 100);

    analogWrite(redPin, newRed);
    analogWrite(greenPin, newGreen);
    analogWrite(bluePin, newBlue);

    if (!inAnimation) {
        currentBlue = newBlue;
        currentRed = newRed;
        currentGreen = newGreen;
    }
}

bool led::isInAnimation() {
    return inAnimation;
}
