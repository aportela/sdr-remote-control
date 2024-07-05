
/*
  a lot of inspiration taken from https://github.com/Potatof/CATui
*/

#define CURRENT_VERSION 0.01

// #define DISPLAY_ST7789_240x320
#define DISPLAY_ILI9488_480x320

// Ai Esp32 Rotary Encoder by Igor Antolic
// https://github.com/igorantolic/ai-esp32-rotary-encoder
#include <AiEsp32RotaryEncoder.h>

#include "Arduino.h"
#include "src/connection/SDRRadio-TS2K/SDRRadioTS2KSerialConnection.hpp"

// #include "src/controls/MainVFORotaryControl.hpp"

// #include "SerialConnection.hpp"
#include "Transceiver.hpp"

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

#ifdef DISPLAY_DRIVER_FOUND
#else
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

uint8_t encoderChangeBitmask = 0;

bool currentVFOFrequencyChanged = true;
volatile uint64_t currentVFOFrequency = 200145;

volatile uint64_t currentSMeterLevel = 0;

// sdrRemoteTransceiver trx;
Transceiver *trx = nullptr;

static uint64_t lastEncoderMillis = millis();

static bool ccw1_fall = false;
static bool cw1_fall = false;

SDRRadioTS2KSerialConnection *serialConnection;
// SerialConnection *serialConnection;

// read & debounce rotary encoder
// code (with some changes) by MostlyMegan: https://reddit.com/r/raspberrypipico/comments/pacarb/sharing_some_c_code_to_read_a_rotary_encoder/

void onEncoderIncrement(uint acceleratedDelta)
{
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

void onEncoderDecrement(uint acceleratedDelta)
{
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

void encoder_callback_ccw(uint pinA, uint pinB)
{
  // trx->setLockedByControls(true);
  uint8_t enc_value_A = digitalRead(pinA);
  uint8_t enc_value_B = digitalRead(pinB);
  uint8_t enc_value = (enc_value_A << 1) | enc_value_B;

  if ((!cw1_fall) && (enc_value == 0b10))
  {
    cw1_fall = true;
  }
  if ((ccw1_fall) && (enc_value == 0b00))
  {
    cw1_fall = false;
    ccw1_fall = false;
    uint8_t delta = 0;
    uint8_t acceleratedDelta = 0;
    uint64_t currentMillis = millis();
    if (currentMillis - lastEncoderMillis < 5)
    {
      acceleratedDelta = 20;
    }
    else if (currentMillis - lastEncoderMillis < 10)
    {
      acceleratedDelta = 10;
    }
    else if (currentMillis - lastEncoderMillis < 20)
    {
      acceleratedDelta = 5;
    }
    else if (currentMillis - lastEncoderMillis < 50)
    {
      acceleratedDelta = 2;
    }
    else
    {
      acceleratedDelta = 1;
    }
    lastEncoderMillis = currentMillis;
    onEncoderDecrement(acceleratedDelta);
  }
  // trx->setLockedByControls(false);
}

void encoder_callback_cw(uint pinA, uint pinB)
{
  // trx->setLockedByControls(true);
  uint8_t enc_value_A = digitalRead(pinA);
  uint8_t enc_value_B = digitalRead(pinB);
  uint8_t enc_value = (enc_value_A << 1) | enc_value_B;

  if ((!ccw1_fall) && (enc_value == 0b01))
  {
    ccw1_fall = true;
  }
  if ((cw1_fall) && (enc_value == 0b00))
  {
    cw1_fall = false;
    ccw1_fall = false;
    uint8_t acceleratedDelta = 0;
    uint64_t currentMillis = millis();
    if (currentMillis - lastEncoderMillis < 5)
    {
      acceleratedDelta = 20;
    }
    else if (currentMillis - lastEncoderMillis < 10)
    {
      acceleratedDelta = 10;
    }
    else if (currentMillis - lastEncoderMillis < 20)
    {
      acceleratedDelta = 5;
    }
    else if (currentMillis - lastEncoderMillis < 50)
    {
      acceleratedDelta = 2;
    }
    else
    {
      acceleratedDelta = 1;
    }
    lastEncoderMillis = currentMillis;
    onEncoderIncrement(acceleratedDelta);
  }
  // trx->setLockedByControls(false);
}

void initRotaryEncoders(void)
{
  pinMode(ENC1_A, INPUT_PULLUP);
  pinMode(ENC1_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC1_A), []()
                  { encoder_callback_ccw(ENC1_A, ENC1_B); }, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC1_B), []()
                  { encoder_callback_cw(ENC1_A, ENC1_B); }, CHANGE);
}

void setup()
{
  serialConnection = new SDRRadioTS2KSerialConnection(&Serial, SERIAL_BAUD_RATE, SERIAL_TIMEOUT);
  initRotaryEncoders();
  // by default encoder changes affect tune
  encoderChangeBitmask |= ENCODER_CHANGE_TUNE;
  trx = new Transceiver();
  // vfo = new MainVFORotaryControl(MAIN_VFO_ROTARY_ENCODER_PIN_A, MAIN_VFO_ROTARY_ENCODER_PIN_B, 0, 0, trx);
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
