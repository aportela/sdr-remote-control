#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2
#define TFT_MOSI 23  // Data out
#define TFT_SCLK 18  // Clock out

#define DISPLAY_WIDTH 320;
#define DISPLAY_HEIGHT 240;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void initDisplay(void) {
  tft.init(240, 320);  // Init ST7789 320x240
  // rotate to 320x240
  tft.setRotation(1);
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
}

void setup() {
  initDisplay();
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.printf("ESP32 SDR Remote Control");
}

void loop() {
}
