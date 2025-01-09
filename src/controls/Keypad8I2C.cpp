#include "Keypad8I2C.hpp"
#include <Arduino.h>

Keypad8I2C::Keypad8I2C(uint8_t i2c_addr, const uint8_t *pinArray, const size_t pinArraySize, const uint16_t debounceMillis, bool allowContinuousPress) : debounceMillis(debounceMillis), allowContinuousPress(allowContinuousPress)
{
    if (pinArray == nullptr || pinArraySize == 0)
    {
        this->pins = nullptr;
        this->pinCount = 0;
        this->lastPinsDebounceMillisecsPtr = nullptr;
        this->buttonStates = nullptr;
        return;
    }
    if (!mcp.begin_I2C(i2c_addr))
    {
        this->pins = nullptr;
        this->pinCount = 0;
        this->lastPinsDebounceMillisecsPtr = nullptr;
        this->buttonStates = nullptr;
        return;
    }
    this->pinCount = pinArraySize;
    this->pins = new uint8_t[this->pinCount];
    this->lastPinsDebounceMillisecsPtr = new uint64_t[this->pinCount];
    this->buttonStates = new bool[this->pinCount]();
    uint64_t currentMillis = millis();
    for (size_t i = 0; i < this->pinCount; i++)
    {
        this->pins[i] = pinArray[i];
        mcp.pinMode(this->pins[i], INPUT_PULLUP);
        this->lastPinsDebounceMillisecsPtr[i] = currentMillis;
    }
}

Keypad8I2C::~Keypad8I2C()
{
    delete[] this->pins;
    this->pins = nullptr;
    delete[] this->lastPinsDebounceMillisecsPtr;
    this->lastPinsDebounceMillisecsPtr = nullptr;
    delete[] this->buttonStates;
    this->buttonStates = nullptr;
}

uint16_t Keypad8I2C::loop(void)
{
    uint16_t pressedButtonsMask = 0;
    if (this->pins == nullptr || this->pinCount == 0 || this->lastPinsDebounceMillisecsPtr == nullptr)
    {
        return pressedButtonsMask;
    }
    uint64_t currentMillis = millis();
    for (size_t i = 0; i < this->pinCount; i++)
    {
        int pinState = mcp.digitalRead(this->pins[i]);
        if (pinState == LOW)
        {
            if (!this->buttonStates[i] && (currentMillis - this->lastPinsDebounceMillisecsPtr[i] > this->debounceMillis))
            {
                this->buttonStates[i] = true;
                this->lastPinsDebounceMillisecsPtr[i] = currentMillis;

                if (!allowContinuousPress)
                {
                    pressedButtonsMask |= (1 << i);
                }
            }
            if (allowContinuousPress && this->buttonStates[i])
            {
                pressedButtonsMask |= (1 << i);
            }
        }
        else
        {
            this->buttonStates[i] = false;
            this->lastPinsDebounceMillisecsPtr[i] = currentMillis;
        }
    }
    return pressedButtonsMask;
}