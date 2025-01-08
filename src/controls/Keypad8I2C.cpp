#include "Keypad8I2C.hpp"
#include <Arduino.h>

Keypad8I2C::Keypad8I2C(uint8_t i2c_addr, const uint8_t *pinArray, const size_t pinArraySize)
{
    if (pinArray == nullptr || pinArraySize == 0)
    {
        this->pins = nullptr;
        this->pinCount = 0;
        return;
    }
    if (!mcp.begin_I2C(i2c_addr))
    {
        this->pins = nullptr;
        this->pinCount = 0;
        return;
    }
    this->pinCount = pinArraySize;
    this->pins = new uint8_t[this->pinCount];
    for (size_t i = 0; i < this->pinCount; i++)
    {
        this->pins[i] = pinArray[i];
        mcp.pinMode(this->pins[i], INPUT_PULLUP);
    }
}

Keypad8I2C::~Keypad8I2C()
{
    delete[] this->pins;
    this->pins = nullptr;
}

uint8_t Keypad8I2C::loop(void)
{
    uint8_t pressedButtonsMask = 0;
    if (this->pins == nullptr || this->pinCount == 0)
    {
        return pressedButtonsMask;
    }
    for (size_t i = 0; i < this->pinCount; i++)
    {
        int pinState = mcp.digitalRead(this->pins[i]);
        if (pinState == LOW)
        {
            pressedButtonsMask |= (1 << i);
        }
    }
    return pressedButtonsMask;
}