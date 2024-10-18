#ifndef SDR_REMOTE_CONTROL_KEYPAD8_CONTROL_H
#define SDR_REMOTE_CONTROL_KEYPAD8_CONTROL_H

#include <stdint.h>
#include <Bounce2.h>

#define KP8_BUTTON_COUNT 8

class Keypad8
{
private:
    static Bounce *KP8Buttons[KP8_BUTTON_COUNT];
    Keypad8() {}

public:
    static void init(const uint8_t (&buttonPins)[KP8_BUTTON_COUNT]);
    void cleanup(void);
    static uint8_t loop(void);
};

#endif // SDR_REMOTE_CONTROL_KEYPAD8_CONTROL_H
