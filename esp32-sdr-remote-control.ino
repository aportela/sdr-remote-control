/*
  a lot of inspiration taken from https://github.com/Potatof/CATui
*/
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
// #include <SPI.h>

// Ai Esp32 Rotary Encoder by Igor Antolic
// https://github.com/igorantolic/ai-esp32-rotary-encoder
#include <AiEsp32RotaryEncoder.h>

#include "Arduino.h"

#include "ts2k_sdrradio_protocol.h"
#include "Display.h"
#include "sdr_remote_transceiver.h"

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2
#define TFT_MOSI 23
#define TFT_SCLK 18

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320
#define DISPLAY_ROTATION 1  // 0 = no rotate, 1 = 90 degrees, 2 = 180 degrees, 3 = 270 degrees (ST7789 has resolution of 240x320, must rotate 90 degrees, or 270 if inversed to allow "panoramic view")

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

#define MAIN_VFO_ROTARY_ENCODER_PIN_A 26
#define MAIN_VFO_ROTARY_ENCODER_PIN_B 25
#define MAIN_VFO_ROTARY_ENCODER_VCC_PIN -1  // put -1 of Rotary encoder Vcc is connected directly to 3,3V
#define MAIN_VFO_ROTARY_ENCODER_STEPS 4     // depending on your encoder - try 1,2 or 4 to get expected behaviour

#define MAIN_VFO_ROTARY_ENCODER_MIN_VALUE 0
#define MAIN_VFO_ROTARY_ENCODER_MAX_VALUE 9999
#define MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE 5000
#define MAIN_VFO_ROTARY_ENCODER_ACCELERATION_VALUE 100

#define CURRENT_VERSION 0.01

bool connected = true;

const char *modes[] = {
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

AiEsp32RotaryEncoder selectFocusRotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_B, ROTARY_ENCODER_A, ROTARY_ENCODER_SWITCH_BUTTON, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder mainVFORotaryEncoder = AiEsp32RotaryEncoder(MAIN_VFO_ROTARY_ENCODER_PIN_A, MAIN_VFO_ROTARY_ENCODER_PIN_B, -1, MAIN_VFO_ROTARY_ENCODER_VCC_PIN, MAIN_VFO_ROTARY_ENCODER_STEPS);

void IRAM_ATTR readEncoderISR() {
  selectFocusRotaryEncoder.readEncoder_ISR();
}

void IRAM_ATTR readBigEncoderISR() {
  mainVFORotaryEncoder.readEncoder_ISR();
}

Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, 1, TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

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

volatile uint64_t currentSMeterLevel = 0;

void initRotaryEncoders(void) {
  selectFocusRotaryEncoder.begin();
  selectFocusRotaryEncoder.setup(readEncoderISR);
  selectFocusRotaryEncoder.setBoundaries(0, 99, true);
  selectFocusRotaryEncoder.disableAcceleration();
  selectFocusRotaryEncoder.setEncoderValue(50);

  mainVFORotaryEncoder.begin();
  mainVFORotaryEncoder.setup(readBigEncoderISR);
  mainVFORotaryEncoder.setBoundaries(MAIN_VFO_ROTARY_ENCODER_MIN_VALUE, MAIN_VFO_ROTARY_ENCODER_MAX_VALUE, true);
  mainVFORotaryEncoder.setAcceleration(MAIN_VFO_ROTARY_ENCODER_ACCELERATION_VALUE);
  mainVFORotaryEncoder.setEncoderValue(MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
}

void setup() {
  initSerial();
  initRotaryEncoders();
}

void tryConnection(void) {
  display.showConnectScreen(SERIAL_BAUD_RATE, CURRENT_VERSION);
  Serial.flush();
  delay(SERIAL_FLUSH_WAIT);
  Serial.print("PS;");
  delay(SERIAL_WAIT_AFTER_SEND_CMD);
  while (Serial.available() > 0 && !connected) {
    String receivedData = Serial.readStringUntil(';');
    if (receivedData == "PS1;") {
      Serial.flush();
      delay(SERIAL_FLUSH_WAIT);
      // tft.fillScreen(ST77XX_BLACK);
      // tft.setCursor(10, 50);
      // tft.printf("MAIN");
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

bool smeterCreated = false;
void showMainScreen(void) {
  if (transmitStatusChanged) {
    display.refreshTransmitStatus(isTransmitting);
    transmitStatusChanged = false;
  }
  if (activeVFOChanged) {
    display.refreshActiveVFO(activeVFO);
    activeVFOChanged = false;
  }
  if (VFOModeChanged) {
    display.refreshVFOMode(VFOMode);
    VFOModeChanged = false;
  }
  if (currentVFOFrequencyChanged) {
    display.refreshVFOFreq(currentVFOFrequency);
    currentVFOFrequencyChanged = false;
  }
  if (!smeterCreated) {
    display.createDigitalSMeter();
    smeterCreated = true;
  }
  display.refreshRNDDigitalSMeter(random(0, 42));
  // display.refreshRNDDigitalSMeter(currentSMeterLevel);
}

void mainVFORotaryEncoderLoop(void) {
  int16_t delta = mainVFORotaryEncoder.encoderChanged();
  if (delta > 0) {
    int32_t newEncoderValue = mainVFORotaryEncoder.readEncoder();
    if (newEncoderValue > 5030) {
      display.debugBottomStr("++++", newEncoderValue - MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
      currentVFOFrequency += 1000;
    } else if (newEncoderValue > 5025) {
      display.debugBottomStr("+++", newEncoderValue - MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
      currentVFOFrequency += 100;
    } else if (newEncoderValue > 5015) {
      display.debugBottomStr("++", newEncoderValue - MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
      currentVFOFrequency += 10;
    } else {
      display.debugBottomStr("+", newEncoderValue - MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
      currentVFOFrequency += 1;
    }
    mainVFORotaryEncoder.setEncoderValue(MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
    currentVFOFrequencyChanged = true;
  } else if (delta < 0) {
    int32_t newEncoderValue = mainVFORotaryEncoder.readEncoder();
    if (newEncoderValue < 4970) {
      display.debugBottomStr("----", newEncoderValue + 5000);
      currentVFOFrequency -= 1000;
    } else if (newEncoderValue < 4975) {
      display.debugBottomStr("---", newEncoderValue + 5000);
      currentVFOFrequency -= 100;
    } else if (newEncoderValue < 4985) {
      display.debugBottomStr("--", newEncoderValue + 5000);
      currentVFOFrequency -= 10;
    } else {
      display.debugBottomStr("-", newEncoderValue + 5000);
      currentVFOFrequency -= 1;
    }
    mainVFORotaryEncoder.setEncoderValue(MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
    currentVFOFrequencyChanged = true;
  }
}

static bool buttonDown = false;
static bool spanChanged = false;
unsigned int spanPosition = 11;
const unsigned int spanPositions[] = { 32, 50, 68, 104, 122, 140, 176, 194, 212, 248, 266, 284 };

unsigned long previousMillis = 0;
// max screen refresh / second
const long interval = 16;  // (33 => 30fps limit, 16 => 60fps limit, 7 => 144 fps limit)
void loop() {
  if (!connected) {
    tryConnection();
  } else {
    /*
    if (selectFocusRotaryEncoder.encoderChanged()) {
      currentVFOFrequency = selectFocusRotaryEncoder.readEncoder();
      currentVFOFrequencyChanged = true;
    }
    */
    if (selectFocusRotaryEncoder.isEncoderButtonDown()) {
      buttonDown = true;
    } else if (buttonDown) {
      buttonDown = false;
      spanChanged = true;
    }
    if (spanChanged) {
      // tft.setCursor(35, 0);
      // tft.drawFastHLine(0, 56, 320, ST77XX_BLACK);
      // tft.drawFastHLine(spanPositions[spanPosition], 56, 18, ST77XX_WHITE);
      spanPosition++;
      if (spanPosition > 11) {
        spanPosition = 0;
      }
      spanChanged = false;
    } else {
      int16_t encoderDelta = selectFocusRotaryEncoder.encoderChanged();
      if (encoderDelta > 0) {
        /*
        if (currentSMeterLevel < 42) {
          currentSMeterLevel++;
        }
        */
        currentVFOFrequency++;
        currentVFOFrequencyChanged = true;
      } else if (encoderDelta < 0) {
        /*
        if (currentSMeterLevel > 0) {
          currentSMeterLevel--;
        }
        */
        currentVFOFrequency--;
        currentVFOFrequencyChanged = true;
      }

      mainVFORotaryEncoderLoop();
    }
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      showMainScreen();
    }
  }
  delay(10);
}
