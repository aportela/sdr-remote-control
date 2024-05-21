/*
  a lot of inspiration taken from https://github.com/Potatof/CATui
*/
// credits:
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>

#include <Arduino.h>

#include "ts2000.h"

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2
#define TFT_MOSI 23
#define TFT_SCLK 18

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define SERIAL_BAUD_RATE 115200
#define SERIAL_TIMEOUT 2000

#define ROTARY_ENCODER_A 34
#define ROTARY_ENCODER_B 35

volatile bool rotaryEncoderChanged = false;
int lastRotaryEncoderAValue = LOW;

bool connected = true;
const char* frames[] = { "|", "/", "-", "\\" };
int8_t currentFrame = -1;

volatile long long currentVFOFreq = 145625000;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void initDisplay(void) {
  tft.init(240, 320);  // Init ST7789 320x240
  // rotate to 320x240
  tft.setRotation(1);
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
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

void IRAM_ATTR handleEncoder() {
  static bool lastAState = false;
  static bool lastBState = false;
  bool currentAState = digitalRead(ROTARY_ENCODER_A);
  bool currentBState = digitalRead(ROTARY_ENCODER_B);
  if (currentAState != lastAState) {
    if (currentBState == currentAState) {
      currentVFOFreq--;
    } else {
      currentVFOFreq++;
    }
  }
  lastAState = currentAState;
  lastBState = currentBState;
}

void initRotaryEncoder(void) {
  pinMode(ROTARY_ENCODER_A, INPUT_PULLUP);
  pinMode(ROTARY_ENCODER_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A), handleEncoder, CHANGE);
}

void setup() {
  initSerial();
  initRotaryEncoder();
  initDisplay();
}

void showConnectScreen(void) {
  if (currentFrame < 0) {
    tft.drawRect(4, 4, 312, 232, ST77XX_WHITE);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setCursor(18, 80);
    tft.printf("ESP32 SDR Remote Control");
    tft.setCursor(140, 100);
    tft.printf("v0.1");
    tft.setCursor(10, 210);
    tft.printf("TS-2000 CAT / %d baud", SERIAL_BAUD_RATE);
    currentFrame = 0;
  }
  tft.setCursor(160, 140);
  tft.printf(frames[currentFrame]);
  currentFrame = (currentFrame + 1) % 4;
}

void showMainScreen(void) {
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.printf("VFOA: %d", currentVFOFreq);
}

void loop() {
  if (!connected) {
    showConnectScreen();
    Serial.flush();
    delay(10);
    Serial.print(TS2K_CMD_POWER_STATUS);
    Serial.println(TS2K_CMD_TERMINATOR);
    delay(10);
    while (Serial.available() > 0 && !connected) {
      String receivedData = Serial.readStringUntil(';');
      if (receivedData == TS2K_CMD_POWER_STATUS_RESPONSE_ON) {
        connected = true;
        Serial.flush();
        tft.fillScreen(ST77XX_BLACK);
      }
    }
    delay(10);
  } else {    
    showMainScreen();
    delay(5);
  }
}
