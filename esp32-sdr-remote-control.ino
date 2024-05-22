/*
  a lot of inspiration taken from https://github.com/Potatof/CATui
*/
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
//#include <SPI.h>

// Ai Esp32 Rotary Encoder by Igor Antolic
// https://github.com/igorantolic/ai-esp32-rotary-encoder
#include "AiEsp32RotaryEncoder.h"

#include "Arduino.h"

#include "ts2000.h"
#include "Display.h"
//#include "transceiver.h"

#define TS2K_SDR_RADIO_CONSOLE

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2
#define TFT_MOSI 23
#define TFT_SCLK 18

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define DISPLAY_INACTIVE_COLOR 0x0841

#define SERIAL_BAUD_RATE 115200
#define SERIAL_TIMEOUT 2000
#define SERIAL_FLUSH_WAIT 10
#define SERIAL_WAIT_AFTER_SEND_CMD 10

#define ROTARY_ENCODER_A 34
#define ROTARY_ENCODER_B 35
#define ROTARY_ENCODER_SWITCH_BUTTON 32
#define ROTARY_ENCODER_VCC_PIN -1  // put -1 of Rotary encoder Vcc is connected directly to 3,3V
#define ROTARY_ENCODER_STEPS 4     // depending on your encoder - try 1,2 or 4 to get expected behaviour

#define CURRENT_VERSION 0.01

//sdrRemoteTransceiver trans;

//initSDRRemoteTransceiver(trans);
//setCurrentHzStep(&trans, 10);

int lastRotaryEncoderAValue = LOW;

bool connected = true;

const char* modes[] = {
  "DSB",
  "LSB",
  "USB",
  "CWU",
  "FM",
  "SAM",
  "",
  "CWL",
  "WFM",
  "BFM",
  "???"
};

uint16_t freqChangeHzStepSize = 12500;  // Hz

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_B, ROTARY_ENCODER_A, ROTARY_ENCODER_SWITCH_BUTTON, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

Display display;

void initDisplay(void) {
  tft.init(DISPLAY_HEIGHT, DISPLAY_WIDTH);
  tft.setRotation(1);
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  display.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void initSerial(void) {
  Serial.setDebugOutput(false);
  Serial.setTimeout(SERIAL_TIMEOUT);
  Serial.clearWriteError();
  Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1);
  while (!Serial) {
    yield();
    delay(10);  // wait for serial port to connect. Needed for native USB port only
  }
}

bool currentVFOFrequencyChanged = true;
volatile uint64_t currentVFOFrequency = 200145;

void initRotaryEncoder(void) {
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(0, 999, true);
  rotaryEncoder.disableAcceleration();
  rotaryEncoder.setEncoderValue(0);
}

void setup() {
  initSerial();
  initDisplay();
  initRotaryEncoder();
}

void tryConnection(void) {
  display.showConnectScreen(&tft, SERIAL_BAUD_RATE, CURRENT_VERSION);
  Serial.flush();
  delay(SERIAL_FLUSH_WAIT);
  Serial.printf("%s%s", TS2K_CMD_POWER_STATUS, TS2K_CMD_TERMINATOR);
  delay(SERIAL_WAIT_AFTER_SEND_CMD);
  while (Serial.available() > 0 && !connected) {
    String receivedData = Serial.readStringUntil(';');
    if (receivedData == TS2K_CMD_POWER_STATUS_RESPONSE_ON) {
      Serial.flush();
      delay(SERIAL_FLUSH_WAIT);
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(10, 50);
      tft.printf("MAIN");
      connected = true;
      break;
    }
  }
}

bool transmitStatusChanged = true;
bool isTransmitting = false;

bool activeVFOChanged = true;
uint8_t activeVFO = 0;


bool VFOModeChanged = true;
uint8_t VFOMode = 4;

void showMainScreen(void) {
  if (transmitStatusChanged) {
    display.refreshTransmitStatus(&tft, isTransmitting);
    transmitStatusChanged = false;
  }
  if (activeVFOChanged) {
    display.refreshActiveVFO(&tft, activeVFO);
    activeVFOChanged = false;
  }
  if (VFOModeChanged) {
    display.refreshVFOMode(&tft, VFOMode);
    VFOModeChanged = false;
  }
  if (currentVFOFrequencyChanged) {
    display.refreshVFOFreq(&tft, currentVFOFrequency);
    currentVFOFrequencyChanged = false;
  }
}

static bool buttonDown = false;
static bool spanChanged = false;
unsigned int spanPosition = 11;
const unsigned int spanPositions[] = { 32, 50, 68, 104, 122, 140, 176, 194, 212, 248, 266, 284 };
void loop() {
  if (!connected) {
    tryConnection();
  } else {
    /*
    if (rotaryEncoder.encoderChanged()) {
      currentVFOFrequency = rotaryEncoder.readEncoder();
      currentVFOFrequencyChanged = true;
    }
    */
    if (rotaryEncoder.isEncoderButtonDown()) {
      buttonDown = true;
    } else if (buttonDown) {
      buttonDown = false;
      spanChanged = true;
    }
    if (spanChanged) {
      tft.setCursor(35, 0);
      tft.drawFastHLine(0, 56, 320, ST77XX_BLACK);
      tft.drawFastHLine(spanPositions[spanPosition], 56, 18, ST77XX_WHITE);
      spanPosition++;
      if (spanPosition > 11) {
        spanPosition = 0;
      }
      spanChanged = false;
    } else {
      int16_t encoderDelta = rotaryEncoder.encoderChanged();
      if (encoderDelta > 0) {
        currentVFOFrequency++;
        currentVFOFrequencyChanged = true;
      } else if (encoderDelta < 0) {
        currentVFOFrequency--;
        currentVFOFrequencyChanged = true;
      }
    }
    showMainScreen();
  }
  delay(10);
}
