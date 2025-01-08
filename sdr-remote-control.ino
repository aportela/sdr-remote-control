/*
  SDR-REMOTE-CONTROL

  a lot of inspiration taken from https://github.com/Potatof/CATui

*/

#include <Arduino.h>
#include "src/DisplayConfiguration.hpp"
#include "src/CommonDefines.hpp"
#include "src/controls/RotaryControl.hpp"
#include "src/controls/Keypad8.hpp"
#include "src/controls/Keypad8I2C.hpp"
#include "src/Transceiver.hpp"
#include "src/MenuConfiguration.hpp"
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

#ifdef DISPLAY_DRIVER_LOVYANGFX

#include "src/display/LGFX.hpp"
LGFX *screen = nullptr;

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANGFX

Transceiver *trx = nullptr;
TransceiverStatus *trxStatus = nullptr;

Menu *menu = nullptr;

// ROTARY ENCODER PINS

#define ENC1_A 32 // 19
#define ENC1_B 33 // 21

#define ENC2_A 13
#define ENC2_B 14

#define ENC3_A 27
#define ENC3_B 26

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

#ifdef DEBUG_DUMMY_CONNECTION

DummyConnection *connection;

#else // DEBUG_DUMMY_CONNECTION

SDRRadioTS2KSerialConnection *connection;

#endif // DEBUG_DUMMY_CONNECTION

#define ENCODER_DEBOUNCE_MS_SHORT 16
#define ENCODER_DEBOUNCE_MS_LONG 48

void onEncoderIncrement(uint8_t acceleratedDelta = 1, uint64_t lastMillis = 0)
{
  if (encoderChangeBitmask & ENCODER_CHANGE_TUNE)
  {

    // TODO: send command to serial connection
#ifdef USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE

#ifdef DEBUG_DUMMY_CONNECTION

    trx->incrementActiveVFOFrequency(acceleratedDelta, true);
    trx->increaseSignalMeter(true);

#else // DEBUG_DUMMY_CONNECTION

    trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_INCREASE_FREQUENCY, acceleratedDelta), true);

#endif // DEBUG_DUMMY_CONNECTION

#else // USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE

#ifdef DEBUG_DUMMY_CONNECTION

    trx->incrementActiveVFOFrequency(1, true);

#else // DEBUG_DUMMY_CONNECTION

    trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_INCREASE_FREQUENCY, 1), true);

#endif // DEBUG_DUMMY_CONNECTION

#endif // USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_VOLUME)
  {

#ifdef DEBUG_DUMMY_CONNECTION

    trx->incrementAFGain(1, true);

#else // DEBUG_DUMMY_CONNECTION

    trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_INCREASE_AF_GAIN, (uint8_t)1), true);

#endif // DEBUG_DUMMY_CONNECTION
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_FILTER_BOTH)
  {

#ifdef DEBUG_DUMMY_CONNECTION

    /*
    trx->VFO[trx->activeVFOIndex].filter.LF += acceleratedDelta;
    trx->VFO[trx->activeVFOIndex].filter.HF += acceleratedDelta;
    */

#endif // DEBUG_DUMMY_CONNECTION
  }
}

void onEncoderDecrement(uint8_t acceleratedDelta = 1, uint64_t lastMillis = 0)
{
  if (encoderChangeBitmask & ENCODER_CHANGE_TUNE)
  {
#ifdef USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE

#ifdef DEBUG_DUMMY_CONNECTION

    trx->decrementActiveVFOFrequency(acceleratedDelta, true);
    trx->decreaseSignalMeter(true);

#else // DEBUG_DUMMY_CONNECTION

    trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_DECREASE_FREQUENCY, acceleratedDelta), true);

#endif // DEBUG_DUMMY_CONNECTION

#else // USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE

#ifdef DEBUG_DUMMY_CONNECTION

    trx->decrementActiveVFOFrequency(1, true);

#else // DEBUG_DUMMY_CONNECTION

    trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_DECREASE_FREQUENCY, 1), true);

#endif // DEBUG_DUMMY_CONNECTION

#endif // USE_ENCODER_ACCELERATION_ON_VFO_FREQUENCY_CHANGE
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_VOLUME)
  {

#ifdef DEBUG_DUMMY_CONNECTION

    trx->decrementAFGain(1, true);

#else // DEBUG_DUMMY_CONNECTION

    trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_DECREASE_AF_GAIN, (uint8_t)1), true);

#endif // DEBUG_DUMMY_CONNECTION
  }
  else if (encoderChangeBitmask & ENCODER_CHANGE_FILTER_BOTH)
  {

#ifdef DEBUG_DUMMY_CONNECTION

    /*
    trx->VFO[trx->activeVFOIndex].filter.LF -= acceleratedDelta;
    trx->VFO[trx->activeVFOIndex].filter.HF -= acceleratedDelta;
    */

#endif // DEBUG_DUMMY_CONNECTION
  }
}

void onKP8Loop(uint8_t pressedMask = 0)
{
  uint8_t currentPage = menu->getCurrentPage();
  // F1
  if (pressedMask & (1 << 0))
  {
    switch (currentPage)
    {
    case 1: // TUNE
      encoderChangeBitmask = ENCODER_CHANGE_TUNE;
      menu->setActive(0, true, true);
      break;
    case 2: // VFO A => B
      menu->click(8);
      trx->copyVFO(0, 1, false);
      break;
    }
  }
  // F2
  if (pressedMask & (1 << 1))
  {
    switch (currentPage)
    {
    case 1: // VOLUME
      encoderChangeBitmask = ENCODER_CHANGE_VOLUME;
      menu->setActive(1, true, true);
      trx->toggleAFSquelchActiveStatus(true);
      break;
    case 2: // VFO B => A
      menu->click(9);
      trx->copyVFO(1, 0, false);
      break;
    }
  }
  // F3
  if (pressedMask & (1 << 2))
  {
    switch (currentPage)
    {
    case 1: // FILTER
      switch (encoderChangeBitmask)
      {
      case ENCODER_CHANGE_TUNE:
      case ENCODER_CHANGE_VOLUME:
      default:
        encoderChangeBitmask = ENCODER_CHANGE_FILTER_BOTH;
        break;
      case ENCODER_CHANGE_FILTER_BOTH:
        encoderChangeBitmask = ENCODER_CHANGE_FILTER_LOW;
        break;
      case ENCODER_CHANGE_FILTER_LOW:
        encoderChangeBitmask = ENCODER_CHANGE_FILTER_HIGH;
        break;
      case ENCODER_CHANGE_FILTER_HIGH:
        encoderChangeBitmask = ENCODER_CHANGE_FILTER_BOTH;
        break;
      }
      menu->setActive(2, true, true);
      break;
    case 2: // --- BAND
      menu->click(10);

#ifdef DEBUG_DUMMY_CONNECTION

      trx->decreaseActiveVFOBand(false);

#else // DEBUG_DUMMY_CONNECTION

      trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_DECREASE_BAND), false);

#endif

      break;
    }
  }
  // F4
  if (pressedMask & (1 << 3))
  {
    switch (currentPage)
    {
    case 1: // SETTINGS
      menu->click(3);
      // TODO: show settings screen
      break;
    case 2: // +++ BAND
      menu->click(11);

#ifdef DEBUG_DUMMY_CONNECTION

      trx->increaseActiveVFOBand(false);

#else // DEBUG_DUMMY_CONNECTION

      trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_INCREASE_BAND), false);

#endif

      break;
    }
  }
  // F5
  if (pressedMask & (1 << 4))
  {
    switch (currentPage)
    {
    case 1: // VFO A/B
      menu->click(4);
      trx->toggleActiveVFO(false);
#ifndef DEBUG_DUMMY_CONNECTION

      trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_ACTIVE_VFO_INDEX_CHANGED), true);

#endif
      break;
    }
  }
  // F6
  if (pressedMask & (1 << 5))
  {
    switch (currentPage)
    {
    case 1: // VFO STEP
      menu->click(5);
      trx->toggleActiveVFOCustomStep(false);
      break;
    }
  }
  // F7
  if (pressedMask & (1 << 6))
  {
    switch (currentPage)
    {
    case 1: // VFO MODE
      menu->click(6);
      trx->toggleActiveVFOMode(false);
#ifndef DEBUG_DUMMY_CONNECTION

      trx->enqueueSyncCommand(new TransceiverSyncCommand(TSCT_TOGGLE_VFO_MODE));

#endif
      break;
    }
  }
  // F8
  if (pressedMask & (1 << 7))
  {
    switch (currentPage)
    {
    case 1: // NEXT PAGE
      menu->click(7);
      menu->nextPage();
      break;

    case 2: // PREVIOUS PAGE
      menu->click(15);
      menu->previousPage();
      break;
    }
  }
}

const char *menuLabels[TOTAL_MENU_ITEMS] = {
    "  TUNE  ", // F1 / PAGE 1 (BUTTON INDEX 0)
    " VOLUME ", // F2 / PAGE 1 (BUTTON INDEX 1)
    " FILTER ", // F3 / PAGE 1 (BUTTON INDEX 2)
    "SETTINGS", // F4 / PAGE 1 (BUTTON INDEX 3)
    "VFO  A/B", // F5 / PAGE 1 (BUTTON INDEX 4)
    "VFO STEP", // F6 / PAGE 1 (BUTTON INDEX 5)
    "VFO MODE", // F7 / PAGE 1 (BUTTON INDEX 6)
    ">>>>>>>>", // F8 / PAGE 1 (BUTTON INDEX 7)
    "VFO A=>B", // F1 / PAGE 2 (BUTTON INDEX 8)
    "VFO B=>A", // F2 / PAGE 2 (BUTTON INDEX 9)
    "--- BAND", // F3 / PAGE 2 (BUTTON INDEX 10)
    "+++ BAND", // F4 / PAGE 2 (BUTTON INDEX 11)
    "        ", // F5 / PAGE 2 (BUTTON INDEX 12)
    "        ", // F6 / PAGE 2 (BUTTON INDEX 13)
    "        ", // F7 / PAGE 2 (BUTTON INDEX 14)
    "<<<<<<<<", // F8 / PAGE 2 (BUTTON INDEX 15)
};

const availableMenuActions menuActions[TOTAL_MENU_ITEMS] = {
    MENU_ACTION_CONTROL_TUNE,           // F1 / PAGE 1 (BUTTON INDEX 0)
    MENU_ACTION_CONTROL_VOLUME,         // F2 / PAGE 1 (BUTTON INDEX 1)
    MENU_ACTION_CONTROL_FILTER,         // F3 / PAGE 1 (BUTTON INDEX 2)
    MENU_ACTION_SETTINGS,               // F4 / PAGE 1 (BUTTON INDEX 3)
    MENU_ACTION_TOGGLE_ACTIVE_VFO,      // F5 / PAGE 1 (BUTTON INDEX 4)
    MENU_ACTION_TOGGLE_ACTIVE_VFO_STEP, // F6 / PAGE 1 (BUTTON INDEX 5)
    MENU_ACTION_TOGGLE_ACTIVE_VFO_MODE, // F7 / PAGE 1 (BUTTON INDEX 6)
    MENU_ACTION_NEXT_PAGE,              // F8 / PAGE 1 (BUTTON INDEX 7)
    MENU_ACTION_COPY_VFO_A_B,           // F1 / PAGE 2 (BUTTON INDEX 8)
    MENU_ACTION_COPY_VFO_B_A,           // F2 / PAGE 2 (BUTTON INDEX 9)
    MENU_ACTION_DECREASE_BAND,          // F3 / PAGE 2 (BUTTON INDEX 10)
    MENU_ACTION_INCREASE_BAND,          // F4 / PAGE 2 (BUTTON INDEX 11)
    MENU_ACTION_NONE,                   // F5 / PAGE 2 (BUTTON INDEX 12)
    MENU_ACTION_NONE,                   // F6 / PAGE 2 (BUTTON INDEX 13)
    MENU_ACTION_NONE,                   // F7 / PAGE 2 (BUTTON INDEX 14)
    MENU_ACTION_PREVIOUS_PAGE,          // F8 / PAGE 2 (BUTTON INDEX 15)
};

RotaryControl *encoder1 = nullptr;
RotaryControl *encoder2 = nullptr;
RotaryControl *encoder3 = nullptr;

Keypad8I2C *kp8 = nullptr;

const uint8_t KP8_I2C_PINS[] = {0, 1, 2, 3, 8, 9, 10, 11};

void setup()
{
#ifdef DEBUG_DUMMY_CONNECTION

  connection = new DummyConnection(&Serial, SERIAL_DEFAULT_BAUD_RATE, SERIAL_TIMEOUT);

#else

  connection = new SDRRadioTS2KSerialConnection(&Serial, SERIAL_DEFAULT_BAUD_RATE, SERIAL_TIMEOUT);

#endif // DEBUG_DUMMY_CONNECTION

  encoder1 = new RotaryControl(ENC1_A, ENC1_B, onEncoderIncrement, onEncoderDecrement, true, ENCODER_DEBOUNCE_MS_SHORT);
  encoder2 = new RotaryControl(ENC2_A, ENC2_B, onEncoderIncrement, onEncoderDecrement, false, ENCODER_DEBOUNCE_MS_LONG);
  encoder2 = new RotaryControl(ENC3_A, ENC3_B, onEncoderIncrement, onEncoderDecrement, false, ENCODER_DEBOUNCE_MS_LONG);
  //   Keypad8::init({KP8_F1_PIN, KP8_F2_PIN, KP8_F3_PIN, KP8_F4_PIN, KP8_F5_PIN, KP8_F6_PIN, KP8_F7_PIN, KP8_F8_PIN});
  kp8 = new Keypad8I2C(0x20, KP8_I2C_PINS, sizeof(KP8_I2C_PINS));
  encoderChangeBitmask |= ENCODER_CHANGE_TUNE;
  trx = new Transceiver();
  trxStatus = new TransceiverStatus;

#ifdef DEBUG_DUMMY_CONNECTION

  trxStatus->AFGain = 50;
  trxStatus->activeVFOIndex = 0;
  trxStatus->VFO[0].frequency = 7084152;
  trxStatus->VFO[0].currentBandIndex = 10;
  trxStatus->VFO[0].frequencyStep = 10;
  trxStatus->VFO[0].mode = TRX_VFO_MD_LSB;
  trxStatus->VFO[0].filter.LF = 200;
  trxStatus->VFO[0].filter.HF = 2000;
  trxStatus->VFO[1].frequency = 145625000;
  trxStatus->VFO[1].currentBandIndex = 27;
  trxStatus->VFO[1].frequencyStep = 10000;
  trxStatus->VFO[1].mode = TRX_VFO_MD_FM;
  trxStatus->VFO[1].filter.LF = 6000;
  trxStatus->VFO[1].filter.HF = 6000;
  trx->setCurrentStatus(trxStatus, false);

#endif

  menu = new Menu(menuLabels, menuActions);
  menu->setActive(0, true, false);

#ifdef DISPLAY_DRIVER_LOVYANGFX

  screen = new LGFX(DISPLAY_PIN_SDA, DISPLAY_PIN_SCL, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_DRIVER_LOVYANGFX_WIDTH, DISPLAY_DRIVER_LOVYANGFX_HEIGHT, DISPLAY_DRIVER_LOVYANGFX_ROTATION, DISPLAY_DRIVER_LOVYANGFX_INVERT_COLORS, trxStatus, menu);
  screen->InitScreen(SCREEN_TYPE_NOT_CONNECTED);

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANGFX
}

long oldPosition = 0;
void loop()
{
  onKP8Loop(kp8->loop());
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

      connection->loop(trx);

#else

      if (connection->isDisconnectedByTimeout())
      {
        trx->setPowerOnStatus(false);
        screen->FlipToScreen(SCREEN_TYPE_NOT_CONNECTED);
      }
      else
      {
        connection->loop(trx);
      }

#endif // DEBUG_DUMMY_CONNECTION
    }
  }
#ifdef DISPLAY_DRIVER_LOVYANGFX

  screen->Refresh(false);

#ifdef DEBUG_FPS

  FPS::Loop(999);

#endif // DEBUG_FPS

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANGFX
}
