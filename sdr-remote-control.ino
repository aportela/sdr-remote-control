/*
  SDR-REMOTE-CONTROL

  a lot of inspiration taken from https://github.com/Potatof/CATui

*/

#include <Arduino.h>
#include "src/DisplayConfiguration.hpp"
#include "src/CommonDefines.hpp"
#include "src/utils/FPS.hpp"

#include "src/display/ScreenType.hpp"

#ifdef DISPLAY_DRIVER_LOVYANN

#include "src/display/LGFX.hpp"

LGFX *screen = nullptr;

#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANN

using namespace aportela::microcontroller::utils;

void setup()
{
  Serial.begin(DEFAULT_SERIAL_SPEED);
  while (!Serial)
  {
    yield();
    delay(10);
  }
#ifdef DISPLAY_DRIVER_LOVYANN
  screen = new LGFX(DISPLAY_PIN_SDA, DISPLAY_PIN_SCL, DISPLAY_PIN_CS, DISPLAY_PIN_DC, DISPLAY_PIN_RST, DISPLAY_DRIVER_LOVYANN_WIDTH, DISPLAY_DRIVER_LOVYANN_HEIGHT, DISPLAY_DRIVER_LOVYANN_ROTATION, DISPLAY_DRIVER_LOVYANN_INVERT_COLORS);
  screen->InitScreen(SCREEN_TYPE_NOT_CONNECTED);
#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANN
}

void loop()
{
#ifdef DISPLAY_DRIVER_LOVYANN
  screen->Refresh();
  FPS::Loop(999);
#else

#error NO DISPLAY DRIVER DEFINED

#endif // DISPLAY_DRIVER_LOVYANN
}