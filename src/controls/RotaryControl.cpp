#include "RotaryControl.hpp"
#include <Arduino.h>

RotaryControl::RotaryControl(uint8_t pinA, uint8_t pinB, RotaryControlCallback clockWiseCallback, RotaryControlCallback counterClockWiseCallback)
    : pinA(pinA), pinB(pinB), ccw1_fall(false), cw1_fall(false), lastEncoderMillis(0)
{
    RCCallbacks[RCC_CLOCKWISE] = clockWiseCallback;
    RCCallbacks[RCC_COUNTERCLOCKWISE] = counterClockWiseCallback;

    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);

    attachInterruptArg(digitalPinToInterrupt(pinA), globalInterruptHandler, this, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(pinB), globalInterruptHandler, this, CHANGE);
}

RotaryControl::~RotaryControl()
{
    detachInterrupt(digitalPinToInterrupt(pinA));
    detachInterrupt(digitalPinToInterrupt(pinB));
}

void RotaryControl::globalInterruptHandler(void *arg)
{
    RotaryControl *instance = static_cast<RotaryControl *>(arg);

    uint8_t enc_value_A = digitalRead(instance->pinA);
    uint8_t enc_value_B = digitalRead(instance->pinB);
    uint8_t enc_value = (enc_value_A << 1) | enc_value_B;

    if ((!instance->cw1_fall) && (enc_value == 0b01))
    {
        instance->ccw1_fall = true;
    }
    if ((!instance->ccw1_fall) && (enc_value == 0b10))
    {
        instance->cw1_fall = true;
    }
    if (instance->cw1_fall && enc_value == 0b00)
    {
        instance->handleInterrupt(RCC_CLOCKWISE);
    }
    else if (instance->ccw1_fall && enc_value == 0b00)
    {
        instance->handleInterrupt(RCC_COUNTERCLOCKWISE);
    }
}

void RotaryControl::handleInterrupt(RotaryControlChange rcChange)
{
    this->cw1_fall = false;
    this->ccw1_fall = false;
    uint64_t currentMillis = millis();
    uint8_t acceleratedDelta = 0;

    if (currentMillis - this->lastEncoderMillis < 5)
        acceleratedDelta = 20;
    else if (currentMillis - this->lastEncoderMillis < 10)
        acceleratedDelta = 10;
    else if (currentMillis - this->lastEncoderMillis < 20)
        acceleratedDelta = 5;
    else if (currentMillis - this->lastEncoderMillis < 50)
        acceleratedDelta = 2;
    else
        acceleratedDelta = 1;

    this->lastEncoderMillis = currentMillis;

    if (RCCallbacks[rcChange])
    {
        RCCallbacks[rcChange](acceleratedDelta, currentMillis);
    }
}
