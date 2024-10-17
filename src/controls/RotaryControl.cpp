#include "RotaryControl.hpp"
#include <Arduino.h>

uint8_t RotaryControl::pinA = 0;
uint8_t RotaryControl::pinB = 0;
RotaryControlCallback RotaryControl::RCCallbacks[2] = {nullptr, nullptr};
volatile bool RotaryControl::ccw1_fall = false;
volatile bool RotaryControl::cw1_fall = false;
volatile uint64_t RotaryControl::lastEncoderMillis = 0;

void IRAM_ATTR RotaryControl::RCInterruptHandler(RotaryControlChange rcChange, uint8_t acceleratedDelta, uint64_t lastEncoderMillis)
{
    if (RotaryControl::RCCallbacks[rcChange])
    {
        RotaryControl::RCCallbacks[rcChange](acceleratedDelta, lastEncoderMillis);
    }
}

//  read & debounce rotary encoder
//  code (with some changes) by MostlyMegan: https://reddit.com/r/raspberrypipico/comments/pacarb/sharing_some_c_code_to_read_a_rotary_encoder/

void IRAM_ATTR RotaryControl::counterClockWiseCallback(uint8_t pinA, uint8_t pinB)
{
    uint8_t enc_value_A = digitalRead(pinA);
    uint8_t enc_value_B = digitalRead(pinB);
    uint8_t enc_value = (enc_value_A << 1) | enc_value_B;

    if ((!RotaryControl::cw1_fall) && (enc_value == 0b10))
    {
        RotaryControl::cw1_fall = true;
    }
    if ((RotaryControl::ccw1_fall) && (enc_value == 0b00))
    {
        RotaryControl::cw1_fall = false;
        RotaryControl::ccw1_fall = false;
        uint8_t delta = 0;
        uint8_t acceleratedDelta = 0;
        uint64_t currentMillis = millis();
        if (currentMillis - RotaryControl::lastEncoderMillis < 5)
        {
            acceleratedDelta = 20;
        }
        else if (currentMillis - RotaryControl::lastEncoderMillis < 10)
        {
            acceleratedDelta = 10;
        }
        else if (currentMillis - RotaryControl::lastEncoderMillis < 20)
        {
            acceleratedDelta = 5;
        }
        else if (currentMillis - RotaryControl::lastEncoderMillis < 50)
        {
            acceleratedDelta = 2;
        }
        else
        {
            acceleratedDelta = 1;
        }
        RotaryControl::lastEncoderMillis = currentMillis;
        RotaryControl::RCInterruptHandler(RCC_COUNTERCLOCKWISE, acceleratedDelta, currentMillis);
    }
}

void IRAM_ATTR RotaryControl::clockWiseCallback(uint8_t pinA, uint8_t pinB)
{
    uint8_t enc_value_A = digitalRead(pinA);
    uint8_t enc_value_B = digitalRead(pinB);
    uint8_t enc_value = (enc_value_A << 1) | enc_value_B;

    if ((!RotaryControl::ccw1_fall) && (enc_value == 0b01))
    {
        RotaryControl::ccw1_fall = true;
    }
    if ((RotaryControl::cw1_fall) && (enc_value == 0b00))
    {
        RotaryControl::cw1_fall = false;
        RotaryControl::ccw1_fall = false;
        uint8_t acceleratedDelta = 0;
        uint64_t currentMillis = millis();
        if (currentMillis - RotaryControl::lastEncoderMillis < 5)
        {
            acceleratedDelta = 20;
        }
        else if (currentMillis - RotaryControl::lastEncoderMillis < 10)
        {
            acceleratedDelta = 10;
        }
        else if (currentMillis - RotaryControl::lastEncoderMillis < 20)
        {
            acceleratedDelta = 5;
        }
        else if (currentMillis - RotaryControl::lastEncoderMillis < 50)
        {
            acceleratedDelta = 2;
        }
        else
        {
            acceleratedDelta = 1;
        }
        RotaryControl::lastEncoderMillis = currentMillis;
        RotaryControl::RCInterruptHandler(RCC_CLOCKWISE, acceleratedDelta, currentMillis);
    }
}

void RotaryControl::init(uint8_t pinA, uint8_t pinB, RotaryControlCallback clockWiseCallback, RotaryControlCallback counterClockWiseCallback)
{
    RotaryControl::pinA = pinA;
    RotaryControl::pinB = pinB;
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    RCCallbacks[RCC_CLOCKWISE] = clockWiseCallback;
    RCCallbacks[RCC_COUNTERCLOCKWISE] = counterClockWiseCallback;

    attachInterrupt(digitalPinToInterrupt(RotaryControl::pinA), []()
                    { RotaryControl::counterClockWiseCallback(RotaryControl::pinA, RotaryControl::pinB); }, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RotaryControl::pinB), []()
                    { RotaryControl::clockWiseCallback(RotaryControl::pinA, RotaryControl::pinB); }, CHANGE);
}
