/*
  SDR-REMOTE-CONTROL

  a lot of inspiration taken from https://github.com/Potatof/CATui

*/

#include <Arduino.h>
#include "src/DisplayConfiguration.hpp"
#include "src/CommonDefines.hpp"
#include "src/controls/RotaryControl.hpp"
#include "src/Transceiver.hpp"

#ifdef DEBUG_FPS

#include "src/utils/FPS.hpp"

#endif

using namespace aportela::microcontroller::utils;

#include "src/connection/SDRRadio-TS2K/SDRRadioTS2KSerialConnection.hpp"

#include "src/display/ScreenType.hpp"

#ifdef DISPLAY_DRIVER_LOVYANN

#include "src/display/LGFX.hpp"

LGFX *screen = nullptr;

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANN

Transceiver *trx;

// ROTARY ENCODER PINS

#define ENC1_A 19
#define ENC1_B 21

// bitmask definitions for encoder changes
#define ENCODER_CHANGE_TUNE (1 << 0)        // 1
#define ENCODER_CHANGE_VOLUME (1 << 1)      // 2
#define ENCODER_CHANGE_FILTER_BOTH (1 << 2) // 4
#define ENCODER_CHANGE_FILTER_LOW (1 << 3)  // 8
#define ENCODER_CHANGE_FILTER_HIGH (1 << 4) // 16

volatile uint8_t encoderChangeBitmask = 0;

uint64_t lastEncoderMillis = 0;

SDRRadioTS2KSerialConnection *serialConnection;

void onEncoderIncrement(uint8_t acceleratedDelta = 1, uint64_t lastMillis = 0)
{
  lastEncoderMillis = millis();
  if (encoderChangeBitmask & ENCODER_CHANGE_TUNE)
  {
    trx->incrementActiveVFOFrequency(acceleratedDelta, true);
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
    trx->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_LOW;
    trx->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_HIGH;
    */
  }
  // trx->changed |= TRX_CFLAG_SEND_CAT;
}

void onEncoderDecrement(uint8_t acceleratedDelta = 1, uint64_t lastMillis = 0)
{
  lastEncoderMillis = millis();
  if (encoderChangeBitmask & ENCODER_CHANGE_TUNE)
  {
    trx->decrementActiveVFOFrequency(acceleratedDelta, true);
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
    trx->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_LOW;
    trx->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_HIGH;
    */
  }
  // trx->changed |= TRX_CFLAG_SEND_CAT;
}

void setup()
{
  Serial.begin(SERIAL_DEFAULT_BAUD_RATE);
  while (!Serial)
  {
    yield();
    delay(10);
  }
  // serialConnection = new SDRRadioTS2KSerialConnection(&Serial, SERIAL_DEFAULT_BAUD_RATE, SERIAL_TIMEOUT);
  RotaryControl::init(ENC1_A, ENC1_B, onEncoderIncrement, onEncoderDecrement);
  encoderChangeBitmask |= ENCODER_CHANGE_TUNE;
  trx = new Transceiver();
#ifdef DISPLAY_DRIVER_LOVYANN

  screen = new LGFX(DISPLAY_PIN_SDA, DISPLAY_PIN_SCL, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_DRIVER_LOVYANN_WIDTH, DISPLAY_DRIVER_LOVYANN_HEIGHT, DISPLAY_DRIVER_LOVYANN_ROTATION, DISPLAY_DRIVER_LOVYANN_INVERT_COLORS, trx);
  screen->InitScreen(SCREEN_TYPE_CONNECTED);

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANN
}

void loop()
{
  TransceiverStatus trxStatus;
  if (trx->getCurrentStatus(&trxStatus))
  {
    if (!trxStatus.poweredOn)
    {
      if (serialConnection->tryConnection(trx))
      {
        trx->setPowerOnStatus(true);
        screen->FlipToScreen(SCREEN_TYPE_CONNECTED);
      }
    }
    else
    {
      /*
      if (serialConnection->isDisconnectedByTimeout())
      {
        trx->setPowerOnStatus(false);
        screen->FlipToScreen(SCREEN_TYPE_NOT_CONNECTED);
      }
      else
      {
        serialConnection->loop(trx, &trxStatus);
      }
      */
    }
  }
#ifdef DISPLAY_DRIVER_LOVYANN

  screen->Refresh(false, &trxStatus);

#ifdef DEBUG_FPS

  FPS::Loop(999);

#endif // DEBUG_FPS

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANN
}
