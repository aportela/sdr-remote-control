/*
  a lot of inspiration taken from https://github.com/Potatof/CATui
*/
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>

#include "ts2000.h"

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

#define CURRENT_VERSION "0.1"

volatile bool rotaryEncoderChanged = false;
int lastRotaryEncoderAValue = LOW;

bool connected = true;
const char* frames[] = { "|", "/", "-", "\\" };
int8_t currentFrame = -1;
#define MAX_FRAMES 4


Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void initDisplay(void) {
  tft.init(DISPLAY_HEIGHT, DISPLAY_WIDTH);
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

bool currentVFOFrequencyChanged = true;
volatile uint64_t currentVFOFrequency = 145625000;

void IRAM_ATTR handleEncoder() {
  static bool lastAState = false;
  static bool lastBState = false;
  bool currentAState = digitalRead(ROTARY_ENCODER_A);
  bool currentBState = digitalRead(ROTARY_ENCODER_B);
  if (currentAState != lastAState) {
    if (currentBState == currentAState) {
      currentVFOFrequency--;
    } else {
      currentVFOFrequency++;
    }
    currentVFOFrequencyChanged = true;
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
    tft.setCursor(18, 10);
    tft.printf("ESP32 SDR Remote Control");
    tft.setCursor(140, 30);
    tft.printf("v%s", CURRENT_VERSION);
    tft.setCursor(10, 210);
    tft.printf("TS-2000 CAT / %d baud", SERIAL_BAUD_RATE);
    currentFrame = 0;
  } else if (currentFrame < MAX_FRAMES) {
    tft.setCursor(160, 110);
    tft.printf(frames[currentFrame]);
    currentFrame = (currentFrame + 1) % MAX_FRAMES;
  }
}

void tryConnection(void) {
  showConnectScreen();
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

void refreshTransmitStatus(bool isTransmitting) {
  if (isTransmitting) {
    tft.drawRect(0, 0, 29, 20, ST77XX_RED);
    tft.setTextColor(ST77XX_RED, ST77XX_BLACK);

  } else {
    tft.drawRect(0, 0, 29, 20, ST77XX_GREEN);
    tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  }
  tft.setCursor(3, 3);
  tft.setTextSize(2);
  tft.print(isTransmitting ? "TX" : "RX");
}

bool activeVFOChanged = true;
uint8_t activeVFO = 0;

void refreshActiveVFO(uint8_t number) {
  tft.drawRect(31, 0, 53, 20, ST77XX_WHITE);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor(34, 3);
  tft.setTextSize(2);
  tft.printf("VFO%d", number);
}

bool VFOModeChanged = true;
uint8_t VFOMode = 4;

void refreshVFOMode(uint8_t mode) {
  tft.drawRect(86, 0, 43, 20, ST77XX_WHITE);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor(89, 3);
  tft.setTextSize(2);
  switch (mode) {
    case 0:
      tft.printf("DSB");
      break;
    case 1:
      tft.printf("LSB");
      break;
    case 2:
      tft.printf("USB");
      break;
    case 3:
      tft.printf("CW_U");  // CW (upper sideband)
      break;
    case 4:
      tft.printf("FM");
      break;
    case 5:
      tft.printf("SAM");  // (synchronous AM, includes ECSS)
      break;
    case 6:
      tft.printf("   ");  // not used
      break;
    case 7:
      tft.printf("CWL");  // CW (lower sideband)
      break;
    case 8:
      tft.printf("WFM");
      break;
    case 9:
      tft.printf("BFM");
      break;
    default:
      tft.printf("???");  // invalid value
      break;
  }
}

void refreshVFOFreq(uint64_t frequency) {
  tft.setTextSize(3);
  char formattedFrequency[16];
  sprintf(
    formattedFrequency,
    "%03lld.%03lld.%03lld.%03lld",
    frequency / 1000000000,
    (frequency % 1000000000) / 1000000,
    (frequency % 1000000) / 1000,
    frequency % 1000
  );
  tft.setCursor(35, 30);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(formattedFrequency);
}

void showMainScreen(void) {
  if (transmitStatusChanged) {
    refreshTransmitStatus(isTransmitting);
    transmitStatusChanged = false;
  }
  if (activeVFOChanged) {
    refreshActiveVFO(activeVFO);
    activeVFOChanged = false;
  }
  if (VFOModeChanged) {
    refreshVFOMode(VFOMode);
    VFOModeChanged = false;
  }
  if (currentVFOFrequencyChanged) {
    refreshVFOFreq(currentVFOFrequency);
    currentVFOFrequencyChanged = false;
  }
}

void loop() {
  if (!connected) {
    tryConnection();
  } else {
    showMainScreen();
  }
  delay(10);
}
