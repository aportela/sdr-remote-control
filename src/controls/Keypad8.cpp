#include "Keypad8.hpp"
#include <Arduino.h>

#define DEBOUNCE_MS_INTERVAL 5

Bounce *Keypad8::KP8Buttons[KP8_BUTTON_COUNT];

void Keypad8::init(const uint8_t (&buttonPins)[KP8_BUTTON_COUNT])
{
    for (int i = 0; i < KP8_BUTTON_COUNT; i++)
    {
        Keypad8::KP8Buttons[i] = new Bounce();
        Keypad8::KP8Buttons[i]->attach(buttonPins[i], INPUT_PULLUP);
        Keypad8::KP8Buttons[i]->interval(DEBOUNCE_MS_INTERVAL);
    }
}

void Keypad8::cleanup(void)
{
    for (int i = 0; i < KP8_BUTTON_COUNT; i++)
    {
        delete Keypad8::KP8Buttons[i];
        Keypad8::KP8Buttons[i] = nullptr; // Limpia el puntero
    }
}

uint8_t Keypad8::loop(void)
{
    uint8_t pressedButtonsMask = 0;
    for (int i = 0; i < KP8_BUTTON_COUNT; i++)
    {
        Keypad8::KP8Buttons[i]->update();

        if (Keypad8::KP8Buttons[i]->fell())
        {
            pressedButtonsMask |= (1 << i);
        }
    }
    return pressedButtonsMask;
}
