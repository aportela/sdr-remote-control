#include "Keypad8.hpp"

static KeyPad8KeyCallback KP8Callbacks[8] = {nullptr};

static void IRAM_ATTR KP8KeyInterruptHandler(KeyPad8FKey KeyPad8FKey)
{
    if (KP8Callbacks[KeyPad8FKey])
    {
        KP8Callbacks[KeyPad8FKey]();
    }
}

// Funciones estáticas para manejar las interrupciones de cada tecla
static void IRAM_ATTR KP8OnkeyF1() { KP8KeyInterruptHandler(KP8_F1); }
static void IRAM_ATTR KP8OnkeyF2() { KP8KeyInterruptHandler(KP8_F2); }
static void IRAM_ATTR KP8OnkeyF3() { KP8KeyInterruptHandler(KP8_F3); }
static void IRAM_ATTR KP8OnkeyF4() { KP8KeyInterruptHandler(KP8_F4); }
static void IRAM_ATTR KP8OnkeyF5() { KP8KeyInterruptHandler(KP8_F5); }
static void IRAM_ATTR KP8OnkeyF6() { KP8KeyInterruptHandler(KP8_F6); }
static void IRAM_ATTR KP8OnkeyF7() { KP8KeyInterruptHandler(KP8_F7); }
static void IRAM_ATTR KP8OnkeyF8() { KP8KeyInterruptHandler(KP8_F8); }

void Keypad8::initKey(KeyPad8FKey FKey, uint8_t pin, KeyPad8KeyCallback callback)
{
    pinMode(pin, INPUT_PULLUP);
    KP8Callbacks[FKey] = callback;

    // Asociar la interrupción al pin correspondiente
    switch (FKey)
    {
    case KP8_F1:
        attachInterrupt(digitalPinToInterrupt(pin), KP8OnkeyF1, RISING);
        break;
    case KP8_F2:
        attachInterrupt(digitalPinToInterrupt(pin), KP8OnkeyF2, RISING);
        break;
    case KP8_F3:
        attachInterrupt(digitalPinToInterrupt(pin), KP8OnkeyF3, RISING);
        break;
    case KP8_F4:
        attachInterrupt(digitalPinToInterrupt(pin), KP8OnkeyF4, RISING);
        break;
    case KP8_F5:
        attachInterrupt(digitalPinToInterrupt(pin), KP8OnkeyF5, RISING);
        break;
    case KP8_F6:
        attachInterrupt(digitalPinToInterrupt(pin), KP8OnkeyF6, RISING);
        break;
    case KP8_F7:
        attachInterrupt(digitalPinToInterrupt(pin), KP8OnkeyF7, RISING);
        break;
    case KP8_F8:
        attachInterrupt(digitalPinToInterrupt(pin), KP8OnkeyF8, RISING);
        break;
    }
}
