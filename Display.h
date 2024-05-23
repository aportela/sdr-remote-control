#ifndef SDR_REMOTE_CONTROL_DISPLAY_H
#define SDR_REMOTE_CONTROL_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

class Display {
public:
  Display(uint16_t width, uint16_t height, uint8_t rotation, int8_t pinCS, int8_t pinDC, int8_t pinMOSI, int8_t pinSCLK, int8_t pinRST);
  void showConnectScreen(uint32_t serialBaudRate, float currentVersion);
  void refreshTransmitStatus(bool isTransmitting);
  void refreshActiveVFO(uint8_t number);
  void refreshVFOMode(uint8_t mode);
  void refreshVFOFreq(uint64_t frequency);
  void createDigitalSMeter();
  void refreshRNDDigitalSMeter(int newSignal);
private:
  uint16_t width;
  uint16_t height;
  Adafruit_ST7789 screen;
  static const char* connectionScreenSpriteFrames[4];
  int8_t connectionScreenSpriteCurrentFrame = -1;
  uint8_t oldSignal;
  uint8_t currentSignal = 0;
#define CONNECTION_SCREEN_SPRITE_FRAME_COUNT = 4
};

#endif
