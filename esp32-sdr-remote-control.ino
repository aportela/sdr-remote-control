
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

#include "src/controls/MainVFORotaryControl.hpp"

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
#define SECONDARY_VFO_ROTARY_ENCODER_A 34
#define SECONDARY_VFO_ROTARY_ENCODER_B 35
#define SECONDARY_VFO_ROTARY_ENCODER_SWITCH_BUTTON 32
#define SECONDARY_VFO_ROTARY_ENCODER_VCC_PIN -1 // put -1 of Rotary encoder Vcc is connected directly to 3,3V
#define SECONDARY_VFO_ROTARY_ENCODER_STEPS 4    // depending on your encoder - try 1,2 or 4 to get expected behaviour

#define SECONDARY_VFO_ROTARY_ENCODER_MIN_VALUE 0
#define SECONDARY_VFO_ROTARY_ENCODER_MAX_VALUE 99
#define SECONDARY_VFO_ROTARY_ENCODER_CENTER_VALUE 50

#define VOLUME_ROTARY_ENCODER_A 33
#define VOLUME_ROTARY_ENCODER_B 27
#define VOLUME_ROTARY_ENCODER_SWITCH_BUTTON 14
#define VOLUME_ROTARY_ENCODER_VCC_PIN -1 // put -1 of Rotary encoder Vcc is connected directly to 3,3V
#define VOLUME_ROTARY_ENCODER_STEPS 4    // depending on your encoder - try 1,2 or 4 to get expected behaviour

#define VOLUME_ROTARY_ENCODER_MIN_VALUE 0
#define VOLUME_ROTARY_ENCODER_MAX_VALUE 99
#define VOLUME_ROTARY_ENCODER_CENTER_VALUE 50

#define MAIN_VFO_ROTARY_ENCODER_PIN_A 26
#define MAIN_VFO_ROTARY_ENCODER_PIN_B 25
#define MAIN_VFO_ROTARY_ENCODER_SWITCH_BUTTON -1
#define MAIN_VFO_SECONDARY_VFO_ROTARY_ENCODER_VCC_PIN -1 // put -1 of Rotary encoder Vcc is connected directly to 3,3V
#define MAIN_VFO_SECONDARY_VFO_ROTARY_ENCODER_STEPS 4    // depending on your encoder - try 1,2 or 4 to get expected behaviour

#define MAIN_VFO_ROTARY_ENCODER_MIN_VALUE 0
#define MAIN_VFO_ROTARY_ENCODER_MAX_VALUE 9999
#define MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE 5000
#define MAIN_VFO_SECONDARY_VFO_ROTARY_ENCODER_ACCELERATION_VALUE 100

#ifdef DISPLAY_DRIVER_FOUND
#else
#error NO_DISPLAY_DRIVER_FOUND
#endif

uint16_t freqChangeHzStepSize = 12500; // Hz

AiEsp32RotaryEncoder mainVFORotaryEncoder = AiEsp32RotaryEncoder(MAIN_VFO_ROTARY_ENCODER_PIN_A, MAIN_VFO_ROTARY_ENCODER_PIN_B, MAIN_VFO_ROTARY_ENCODER_SWITCH_BUTTON, MAIN_VFO_SECONDARY_VFO_ROTARY_ENCODER_VCC_PIN, MAIN_VFO_SECONDARY_VFO_ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder secondaryVFORotaryEncoder = AiEsp32RotaryEncoder(SECONDARY_VFO_ROTARY_ENCODER_B, SECONDARY_VFO_ROTARY_ENCODER_A, SECONDARY_VFO_ROTARY_ENCODER_SWITCH_BUTTON, SECONDARY_VFO_ROTARY_ENCODER_VCC_PIN, SECONDARY_VFO_ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder volumeRotaryEncoder = AiEsp32RotaryEncoder(VOLUME_ROTARY_ENCODER_B, VOLUME_ROTARY_ENCODER_A, VOLUME_ROTARY_ENCODER_SWITCH_BUTTON, SECONDARY_VFO_ROTARY_ENCODER_VCC_PIN, SECONDARY_VFO_ROTARY_ENCODER_STEPS);

void IRAM_ATTR readVolumeEncoderISR()
{
  volumeRotaryEncoder.readEncoder_ISR();
}

void IRAM_ATTR readEncoderISR()
{
  secondaryVFORotaryEncoder.readEncoder_ISR();
}

void IRAM_ATTR readBigEncoderISR()
{
  mainVFORotaryEncoder.readEncoder_ISR();
}

#ifdef DISPLAY_ST7789_240x320
DisplayST7789 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_ROTATION, TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
#endif

#ifdef DISPLAY_ILI9488_480x320
DisplayILI9488 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_ROTATION, DISPLAY_INVERT_COLORS);
#endif

bool currentVFOFrequencyChanged = true;
volatile uint64_t currentVFOFrequency = 200145;

volatile uint64_t currentSMeterLevel = 0;

// sdrRemoteTransceiver trx;
Transceiver *trx = nullptr;

MainVFORotaryControl *vfo = nullptr;

void initRotaryEncoders(void)
{

  volumeRotaryEncoder.begin();
  volumeRotaryEncoder.setup(readVolumeEncoderISR);
  volumeRotaryEncoder.setBoundaries(SECONDARY_VFO_ROTARY_ENCODER_MIN_VALUE, SECONDARY_VFO_ROTARY_ENCODER_MAX_VALUE, true);
  volumeRotaryEncoder.setEncoderValue(SECONDARY_VFO_ROTARY_ENCODER_CENTER_VALUE);

  secondaryVFORotaryEncoder.begin();
  secondaryVFORotaryEncoder.setup(readEncoderISR);
  secondaryVFORotaryEncoder.setBoundaries(SECONDARY_VFO_ROTARY_ENCODER_MIN_VALUE, SECONDARY_VFO_ROTARY_ENCODER_MAX_VALUE, true);
  secondaryVFORotaryEncoder.disableAcceleration();
  secondaryVFORotaryEncoder.setEncoderValue(SECONDARY_VFO_ROTARY_ENCODER_CENTER_VALUE);

  mainVFORotaryEncoder.begin();
  mainVFORotaryEncoder.setup(readBigEncoderISR);
  mainVFORotaryEncoder.setBoundaries(MAIN_VFO_ROTARY_ENCODER_MIN_VALUE, MAIN_VFO_ROTARY_ENCODER_MAX_VALUE, true);
  mainVFORotaryEncoder.setAcceleration(MAIN_VFO_SECONDARY_VFO_ROTARY_ENCODER_ACCELERATION_VALUE);
  mainVFORotaryEncoder.setEncoderValue(MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
}

// SDRRadioTS2KSerialConnection *serialConnection;
SerialConnection *serialConnection;

void setup()
{
  serialConnection = new SDRRadioTS2KSerialConnection(&Serial, SERIAL_BAUD_RATE, SERIAL_TIMEOUT);
  // serialConnection = new SerialConnection(&Serial, SERIAL_BAUD_RATE, SERIAL_TIMEOUT);
  initRotaryEncoders();
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

void mainVFORotaryEncoderLoop(void)
{
  int16_t delta = mainVFORotaryEncoder.encoderChanged();
  if (delta > 0)
  {
    int32_t newEncoderValue = mainVFORotaryEncoder.readEncoder();
    int32_t hzIncrement = 0;
    if (newEncoderValue > 5030)
    {
      hzIncrement = 1000;
    }
    else if (newEncoderValue > 5025)
    {
      hzIncrement = 100;
    }
    else if (newEncoderValue > 5015)
    {
      hzIncrement = 100;
    }
    else
    {
      hzIncrement = 1;
    }
    currentVFOFrequency += hzIncrement;
    currentVFOFrequencyChanged = true;
    trx->VFO[trx->activeVFOIndex].frequency += hzIncrement;
    trx->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
    mainVFORotaryEncoder.setEncoderValue(MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
  }
  else if (delta < 0)
  {
    int32_t newEncoderValue = mainVFORotaryEncoder.readEncoder();
    int32_t hzDecrement = 0;
    if (newEncoderValue < 4970)
    {
      // display.debugBottomStr("----", newEncoderValue + 5000);
      hzDecrement = 1000;
    }
    else if (newEncoderValue < 4975)
    {
      // display.debugBottomStr("---", newEncoderValue + 5000);
      hzDecrement = 100;
    }
    else if (newEncoderValue < 4985)
    {
      // display.debugBottomStr("--", newEncoderValue + 5000);
      hzDecrement = 10;
    }
    else
    {
      // display.debugBottomStr("-", newEncoderValue + 5000);
      hzDecrement = 1;
    }
    currentVFOFrequency -= hzDecrement;
    currentVFOFrequencyChanged = true;
    trx->VFO[trx->activeVFOIndex].frequency -= hzDecrement;
    trx->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
    mainVFORotaryEncoder.setEncoderValue(MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
  }
}

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
    serialConnection->loop(trx);
    display.refreshMainScreen(trx);
    if (secondaryVFORotaryEncoder.isEncoderButtonDown())
    {
      buttonDown = true;
    }
    else if (buttonDown)
    {
      buttonDown = false;
      spanChanged = true;
    }
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

      mainVFORotaryEncoderLoop();

      // re-connect on null activity / timeouts ?
      if (trx->poweredOn && serialConnection->isDisconnectedByTimeout())
      {
        // trx->poweredOn = false;
        // display.showConnectScreen(SERIAL_BAUD_RATE, CURRENT_VERSION);
      }
    }
  }
}
