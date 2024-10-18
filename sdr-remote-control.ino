/*
  SDR-REMOTE-CONTROL

  a lot of inspiration taken from https://github.com/Potatof/CATui

*/

#include <Arduino.h>
#include "src/DisplayConfiguration.hpp"
#include "src/CommonDefines.hpp"
#include "src/controls/RotaryControl.hpp"
#include "src/controls/Keypad8.hpp"
#include "src/Transceiver.hpp"
#include "src/Menu.hpp"

#define DEBUG_DUMMY_CONNECTION // this define is for screen test/debuggint purposes only, ignore cat serial data & uses random values

#ifdef DEBUG_FPS

#include "src/utils/FPS.hpp"

#endif // DEBUG_FPS

using namespace aportela::microcontroller::utils;

#ifdef DEBUG_DUMMY_CONNECTION

#include "src/connection/dummy/DummyConnection.hpp"

#else

#include "src/connection/SDRRadio-TS2K/SDRRadioTS2KSerialConnection.hpp"

#endif // DEBUG_DUMMY_CONNECTION

#include "src/display/ScreenType.hpp"

#ifdef DISPLAY_DRIVER_LOVYANN

#include "src/display/LGFX.hpp"
LGFX *screen = nullptr;

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANN

Transceiver *trx = nullptr;
TransceiverStatus *trxStatus = nullptr;

Menu *menu = nullptr;

// ROTARY ENCODER PINS

#define ENC1_A 19
#define ENC1_B 21

// KEYPAD8 PINS

#define KP8_F1_PIN 13
#define KP8_F2_PIN 12
#define KP8_F3_PIN 14
#define KP8_F4_PIN 27
#define KP8_F5_PIN 26
#define KP8_F6_PIN 25
#define KP8_F7_PIN 33
#define KP8_F8_PIN 32

// bitmask definitions for encoder changes
#define ENCODER_CHANGE_TUNE (1 << 0)        // 1
#define ENCODER_CHANGE_VOLUME (1 << 1)      // 2
#define ENCODER_CHANGE_FILTER_BOTH (1 << 2) // 4
#define ENCODER_CHANGE_FILTER_LOW (1 << 3)  // 8
#define ENCODER_CHANGE_FILTER_HIGH (1 << 4) // 16

#define USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE // uncomment this for use acceleration on encoder changes with VFO frequency

volatile uint8_t encoderChangeBitmask = 0;

volatile uint64_t lastEncoderMillis = 0;

#ifdef DEBUG_DUMMY_CONNECTION

DummyConnection *connection;

#else

SDRRadioTS2KSerialConnection *connection;

#endif // DEBUG_DUMMY_CONNECTION

void onEncoderIncrement(uint8_t acceleratedDelta = 1, uint64_t lastMillis = 0)
{
  lastEncoderMillis = millis();
  if (encoderChangeBitmask & ENCODER_CHANGE_TUNE)
  {
#ifdef USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE

    trx->incrementActiveVFOFrequency(acceleratedDelta, true);

#else

    trx->incrementActiveVFOFrequency(1, true);

#endif // USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_VOLUME)
  {
    trx->incrementAFGain(1, true);
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_FILTER_BOTH)
  {
    /*
    trx->VFO[trx->activeVFOIndex].LF += acceleratedDelta;
    trx->VFO[trx->activeVFOIndex].HF += acceleratedDelta;
    */
  }
}

void onEncoderDecrement(uint8_t acceleratedDelta = 1, uint64_t lastMillis = 0)
{
  lastEncoderMillis = millis();
  if (encoderChangeBitmask & ENCODER_CHANGE_TUNE)
  {
#ifdef USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE

    trx->decrementActiveVFOFrequency(acceleratedDelta, true);

#else

    trx->decrementActiveVFOFrequency(1, true);

#endif // USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_VOLUME)
  {
    trx->decrementAFGain(1, true);
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_FILTER_BOTH)
  {
    /*
    trx->VFO[trx->activeVFOIndex].LF -= acceleratedDelta;
    trx->VFO[trx->activeVFOIndex].HF -= acceleratedDelta;
    */
  }
}

void onKP8Loop(uint8_t pressedMask = 0)
{
  // F1
  if (pressedMask & (1 << 0))
  {
    Serial.println("F1");
    encoderChangeBitmask = ENCODER_CHANGE_TUNE;
  }
  // F2
  if (pressedMask & (1 << 1))
  {
    Serial.println("F2");
    encoderChangeBitmask = ENCODER_CHANGE_VOLUME;
  }
  // F3
  if (pressedMask & (1 << 2))
  {
    Serial.println("F3");
  }
  // F4
  if (pressedMask & (1 << 3))
  {
    Serial.println("F4");
  }
  // F5
  if (pressedMask & (1 << 4))
  {
    Serial.println("F5");
    trx->toggleActiveVFO(false);
  }
  // F6
  if (pressedMask & (1 << 5))
  {
    Serial.println("F6");
    trx->toggleActiveVFOCustomStep(false);
  }
  // F7
  if (pressedMask & (1 << 6))
  {
    Serial.println("F7");
    trx->toggleActiveVFOMode(false);
  }
  // F8
  if (pressedMask & (1 << 7))
  {
    Serial.println("F8");
  }
}

void setup()
{
#ifdef DEBUG_DUMMY_CONNECTION

  connection = new DummyConnection(&Serial, SERIAL_DEFAULT_BAUD_RATE, SERIAL_TIMEOUT);

#else

  connection = new SDRRadioTS2KSerialConnection(&Serial, SERIAL_DEFAULT_BAUD_RATE, SERIAL_TIMEOUT);

#endif // DEBUG_DUMMY_CONNECTION

  RotaryControl::init(ENC1_A, ENC1_B, onEncoderIncrement, onEncoderDecrement);
  Keypad8::init({KP8_F1_PIN, KP8_F2_PIN, KP8_F3_PIN, KP8_F4_PIN, KP8_F5_PIN, KP8_F6_PIN, KP8_F7_PIN, KP8_F8_PIN});
  encoderChangeBitmask |= ENCODER_CHANGE_TUNE;
  trx = new Transceiver();
  trxStatus = new TransceiverStatus;
  menu = new Menu();

#ifdef DISPLAY_DRIVER_LOVYANN

  screen = new LGFX(DISPLAY_PIN_SDA, DISPLAY_PIN_SCL, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_DRIVER_LOVYANN_WIDTH, DISPLAY_DRIVER_LOVYANN_HEIGHT, DISPLAY_DRIVER_LOVYANN_ROTATION, DISPLAY_DRIVER_LOVYANN_INVERT_COLORS, trxStatus, menu);
  screen->InitScreen(SCREEN_TYPE_NOT_CONNECTED);

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANN
}

void loop()
{
  onKP8Loop(Keypad8::loop());
  if (trx->getCurrentStatus(trxStatus))
  {
    if (!trxStatus->poweredOn)
    {
      if (connection->tryConnection(trx))
      {
        trx->setPowerOnStatus(true);
        screen->FlipToScreen(SCREEN_TYPE_CONNECTED);
      }
    }
    else
    {
#ifdef DEBUG_DUMMY_CONNECTION

      connection->loop(trx, trxStatus);

#else

      if (connection->isDisconnectedByTimeout())
      {
        trx->setPowerOnStatus(false);
        screen->FlipToScreen(SCREEN_TYPE_NOT_CONNECTED);
      }
      else
      {
        connection->loop(trx, trxStatus);
      }

#endif // DEBUG_DUMMY_CONNECTION
    }
  }
#ifdef DISPLAY_DRIVER_LOVYANN

  screen->Refresh(false);

#ifdef DEBUG_FPS

  FPS::Loop(999);

#endif // DEBUG_FPS

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANN
}
