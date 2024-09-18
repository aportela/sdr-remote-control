
/*
  a lot of inspiration taken from https://github.com/Potatof/CATui
*/

#define CURRENT_VERSION 0.01

// #define DISPLAY_ST7789_240x320
#define DISPLAY_ILI9488_480x320

#include "Arduino.h"
#include "src/connection/SDRRadio-TS2K/SDRRadioTS2KSerialConnection.hpp"

// #include "SerialConnection.hpp"
#include "src/Transceiver.hpp"

#include "src/controls/Keypad8.hpp"

#include "src/controls/RotaryControl.hpp"

#define FUNCUBE_DONGLE_MIN_FREQ 150000     // 150 Khz
#define FUNCUBE_DONGLE_MAX_FREQ 1900000000 // 1.9 Ghz

#define SDR_PLAY_RSPDUO_MIN_FREQ 1000       // 1Khz
#define SDR_PLAY_RSPDUO_MAX_FREQ 2000000000 // 2 Ghz

#define MIN_FREQUENCY FUNCUBE_DONGLE_MIN_FREQ
#define MAX_FREQUENCY FUNCUBE_DONGLE_MAX_FREQ

#define MIN_VOLUME 0
#define MAX_VOLUME 100
#define DEFAULT_VOLUME 50

#ifdef DISPLAY_ST7789_240x320
#include "src/display/ST7789/DisplayST7789.hpp"
#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2
#define TFT_MOSI 23
#define TFT_SCLK 18
#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320
#define DISPLAY_ROTATION 1 // 0 = no rotate, 1 = 90 degrees, 2 = 180 degrees, 3 = 270 degrees (ST7789 has resolution of 240x320, must rotate 90 degrees, or 270 if inversed to allow "panoramic view")
#define DISPLAY_DRIVER_FOUND
#endif

#ifdef DISPLAY_ILI9488_480x320
#include "src/display/ILI9488/Display-ILI9488-320x480.hpp"
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 480
#define DISPLAY_ROTATION 1         // 0 = no rotate, 1 = 90 degrees, 2 = 180 degrees, 3 = 270 degrees (ILI9488 has resolution of 320x480, must rotate 90 degrees, or 270 if inversed to allow "panoramic view")
#define DISPLAY_INVERT_COLORS true // my screen by default inverts colors
#define DISPLAY_DRIVER_FOUND
#endif

// #include "ts2k_sdrradio_protocol.h"

#define SERIAL_BAUD_RATE 57600
#define SERIAL_TIMEOUT 2000
#define SERIAL_FLUSH_WAIT 10
#define SERIAL_WAIT_AFTER_SEND_CMD 10

// TODO: 3 rotary encoders volume / filter/ custom VFO step
// TODO: 3 keys VFO A/B, toggle Mode, toggle Band

#define ENC1_A 19
#define ENC1_B 21

#ifndef DISPLAY_DRIVER_FOUND
#error NO_DISPLAY_DRIVER_FOUND
#endif

uint16_t freqChangeHzStepSize = 12500; // Hz

#ifdef DISPLAY_ST7789_240x320
DisplayST7789 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_ROTATION, TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
#endif

#ifdef DISPLAY_ILI9488_480x320
DisplayILI9488 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_ROTATION, DISPLAY_INVERT_COLORS);
#endif

// bitmask definitions for encoder changes
#define ENCODER_CHANGE_TUNE (1 << 0)        // 1
#define ENCODER_CHANGE_VOLUME (1 << 1)      // 2
#define ENCODER_CHANGE_FILTER_BOTH (1 << 2) // 4
#define ENCODER_CHANGE_FILTER_LOW (1 << 3)  // 8
#define ENCODER_CHANGE_FILTER_HIGH (1 << 4) // 16

volatile uint8_t encoderChangeBitmask = 0;

bool currentVFOFrequencyChanged = true;
volatile uint64_t currentVFOFrequency = 200145;

volatile uint64_t currentSMeterLevel = 0;

// sdrRemoteTransceiver trx;
Transceiver *trx = nullptr;

SDRRadioTS2KSerialConnection *serialConnection;
// SerialConnection *serialConnection;

uint64_t lastEncoderMillis = 0;

void onEncoderIncrement(uint8_t acceleratedDelta = 1, uint64_t lastMillis = 0)
{
  lastEncoderMillis = millis();
  if (encoderChangeBitmask & ENCODER_CHANGE_TUNE)
  {
    trx->incrementActiveVFOFrequency(acceleratedDelta);
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_VOLUME)
  {
    trx->incrementAFGain(1);
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_FILTER_BOTH)
  {
    trx->VFO[trx->activeVFOIndex].LF += acceleratedDelta;
    trx->VFO[trx->activeVFOIndex].HF += acceleratedDelta;
    trx->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_LOW;
    trx->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_HIGH;
  }
  trx->changed |= TRX_CFLAG_SEND_CAT;
}

void onEncoderDecrement(uint8_t acceleratedDelta = 1, uint64_t lastMillis = 0)
{
  lastEncoderMillis = millis();
  if (encoderChangeBitmask & ENCODER_CHANGE_TUNE)
  {
    trx->decrementActiveVFOFrequency(acceleratedDelta);
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_VOLUME)
  {
    trx->decrementAFGain(1);
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_FILTER_BOTH)
  {
    trx->VFO[trx->activeVFOIndex].LF -= acceleratedDelta;
    trx->VFO[trx->activeVFOIndex].HF -= acceleratedDelta;
    trx->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_LOW;
    trx->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_HIGH;
  }
  trx->changed |= TRX_CFLAG_SEND_CAT;
}

void initRotaryEncoders(void)
{
  RotaryControl::init(ENC1_A, ENC1_B, onEncoderIncrement, onEncoderDecrement);
}

void F1(void)
{
  display.setButtonEnabled(0);
  display.setButtonDisabled(1);
  display.setButtonDisabled(2);
  encoderChangeBitmask &= ~ENCODER_CHANGE_VOLUME;
  encoderChangeBitmask |= ENCODER_CHANGE_TUNE;
}

void F2(void)
{
  display.setButtonDisabled(0);
  display.setButtonEnabled(1);
  display.setButtonDisabled(2);
  encoderChangeBitmask &= ~ENCODER_CHANGE_TUNE;
  encoderChangeBitmask |= ENCODER_CHANGE_VOLUME;
}

void F3(void)
{
  display.setButtonDisabled(0);
  display.setButtonDisabled(1);
  display.setButtonEnabled(2);
  encoderChangeBitmask &= ~ENCODER_CHANGE_TUNE;
  encoderChangeBitmask &= ~ENCODER_CHANGE_VOLUME;
}

void initKeys(void)
{
  Keypad8::initKey(KP8_F1, 12, F1);
  Keypad8::initKey(KP8_F2, 13, F2);
  Keypad8::initKey(KP8_F3, 14, F3);
}

void setup()
{
  serialConnection = new SDRRadioTS2KSerialConnection(&Serial, SERIAL_BAUD_RATE, SERIAL_TIMEOUT);
  initRotaryEncoders();
  initKeys();
  // by default encoder changes affect tune
  encoderChangeBitmask |= ENCODER_CHANGE_TUNE;
  trx = new Transceiver();
  //  TODO position bug after change connect->main screen
  //  display.showConnectScreen(SERIAL_BAUD_RATE, CURRENT_VERSION);
  //   display.hideConnectScreen();
  display.showMainScreen();
}

bool transmitStatusChanged = true;
bool isTransmitting = false;

bool activeVFOChanged = true;

uint8_t activeVFO = 0;

bool VFOModeChanged = true;
uint8_t VFOMode = 4;

bool smeterCreated = false;

static bool buttonDown = false;
static bool spanChanged = false;

unsigned long previousMillis = 0;
// max screen refresh / second
const long interval = 16; // (33 => 30fps limit, 16 => 60fps limit, 7 => 144 fps limit)

uint64_t frameCount = 0;
uint64_t lastTime = 0;
float fps = 0;

void loop()
{
  if (!trx->poweredOn)
  {
    display.refreshConnectScreen();
    // clear screen & drawn default connect screen at start (ONLY)
    /*
    if (serialConnection->tryConnection(trx))
    {
      trx->poweredOn = true;
      display.hideConnectScreen();
      display.showMainScreen();
    }
    */
  }
  else
  {
    if (millis() - lastEncoderMillis < 50)
    {
      serialConnection->syncLocalToRemote(trx);
    }
    else
    {
      serialConnection->loop(trx);
    }
    display.refreshMainScreen(trx);
    /*
    if (secondaryVFORotaryEncoder.isEncoderButtonDown())
    {
      buttonDown = true;
    }
    else if (buttonDown)
    {
      buttonDown = false;
      spanChanged = true;
    }
    */
    if (spanChanged)
    {
      if (trx->VFO[trx->activeVFOIndex].customStep > 1)
      {
        trx->VFO[trx->activeVFOIndex].customStep /= 10;
      }
      else
      {
        trx->VFO[trx->activeVFOIndex].customStep = 100000000000;
      }
      trx->changed |= TRX_CFLAG_ACTIVE_VFO_STEP;
      spanChanged = false;
    }
    else
    {

      /*
      int16_t volumeEncoderDelta = volumeRotaryEncoder.encoderChanged();
      if (volumeEncoderDelta > 0)
      {
        if (trx->AFGain < MAX_VOLUME)
        {
          trx->AFGain += 1;
          trx->changed |= TRX_CFLAG_AF_GAIN;
        }
        volumeRotaryEncoder.setEncoderValue(VOLUME_ROTARY_ENCODER_CENTER_VALUE);
      }
      else if (volumeEncoderDelta < 0)
      {
        if (trx->AFGain > MIN_VOLUME)
        {
          trx->AFGain -= 1;
          trx->changed |= TRX_CFLAG_AF_GAIN;
        }
        volumeRotaryEncoder.setEncoderValue(SECONDARY_VFO_ROTARY_ENCODER_CENTER_VALUE);
      }

      int16_t encoderDelta = secondaryVFORotaryEncoder.encoderChanged();
      if (encoderDelta > 0)
      {
        if (trx->VFO[trx->activeVFOIndex].frequency + trx->VFO[trx->activeVFOIndex].customStep <= MAX_FREQUENCY)
        {
          trx->VFO[trx->activeVFOIndex].frequency += trx->VFO[trx->activeVFOIndex].customStep;
          trx->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
        }
        secondaryVFORotaryEncoder.setEncoderValue(SECONDARY_VFO_ROTARY_ENCODER_CENTER_VALUE);
      }
      else if (encoderDelta < 0)
      {
        if (trx->VFO[trx->activeVFOIndex].frequency >= trx->VFO[trx->activeVFOIndex].customStep)
        {
          trx->VFO[trx->activeVFOIndex].frequency -= trx->VFO[trx->activeVFOIndex].customStep;
          trx->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
        }
        secondaryVFORotaryEncoder.setEncoderValue(SECONDARY_VFO_ROTARY_ENCODER_CENTER_VALUE);
      }

      */

      // re-connect on null activity / timeouts ?
      if (trx->poweredOn && serialConnection->isDisconnectedByTimeout())
      {
        // trx->poweredOn = false;
        // display.showConnectScreen(SERIAL_BAUD_RATE, CURRENT_VERSION);
      }
    }
  }
}
