#ifndef SDR_REMOTE_CONTROL_KEY_MENU_CONTROL_H
#define SDR_REMOTE_CONTROL_KEY_MENU_CONTROL_H

#include <Arduino.h>
#include <stdint.h>

enum KeyPad8FKey
{
    KP8_F1,
    KP8_F2,
    KP8_F3,
    KP8_F4,
    KP8_F5,
    KP8_F6,
    KP8_F7,
    KP8_F8
};

typedef void (*KeyPad8KeyCallback)();

class Keypad8
{
public:
    static void initKey(KeyPad8FKey FKey, uint8_t pin, KeyPad8KeyCallback callback);
};

#endif