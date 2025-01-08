

#ifndef SDR_REMOTE_CONTROL_DISPLAY_CONFIGURATION_H
#define SDR_REMOTE_CONTROL_DISPLAY_CONFIGURATION_H

#define DEBUG_SCREEN_WIDGETS_BOUNDS_BORDER_COLOR TFT_WHITE
// #define DEBUG_SCREEN_BOUNDS
// #define DEBUG_SCREEN_WIDGETS_BOUNDS
#define DEBUG_FPS

#define DISPLAY_DRIVER_LOVYANGFX // at this time only LovyanGFX driver is supported

#define DEFAULT_FONT nullptr
#define DEFAULT_FONT_SIZE_1_WIDTH 6
#define DEFAULT_FONT_SIZE_1_HEIGHT 7
#define DEFAULT_FONT_SIZE_2_WIDTH 12
#define DEFAULT_FONT_SIZE_2_HEIGHT 14
#define DEFAULT_FONT_SIZE_3_WIDTH 18
#define DEFAULT_FONT_SIZE_3_HEIGHT 21
#define DEFAULT_FONT_SIZE_4_WIDTH 24
#define DEFAULT_FONT_SIZE_4_HEIGHT 28
#define DEFAULT_FONT_SIZE_5_WIDTH 30
#define DEFAULT_FONT_SIZE_5_HEIGHT 35

#define DEFAULT_TEXT_COLOR TFT_WHITE
#define DEFAULT_BACKGROUND_COLOR TFT_BLACK
#define TEXT_COLOR_ACTIVE DEFAULT_TEXT_COLOR
#define TEXT_COLOR_SECONDARY 0x528A
#define TEXT_COLOR_NOT_ACTIVE 0x18C3
#define TEXT_BACKGROUND_COLOR DEFAULT_BACKGROUND_COLOR // TODO: remove ?
#define COLOR_GREEN TFT_GREEN
#define COLOR_RED TFT_RED

#define DISPLAY_LOVYANGFX_ILI9488_480x320 // uncomment this for using a ILI9488 screen with 480x320 resolution
// #define DISPLAY_LOVYANGFX_ST7789_240x320 // uncomment this for using a ST7789 screen with 240x320 resolution

#ifdef DISPLAY_LOVYANGFX_ILI9488_480x320

#define DISPLAY_PIN_CS 5
#define DISPLAY_PIN_RST 2
#define DISPLAY_PIN_DC 4
#define DISPLAY_PIN_SDA 23
#define DISPLAY_PIN_SCL 18

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320
#define DISPLAY_PADDING 1 // real draw starts/ends contained in this horizontal/vertical (pixel) padding (TODO: WORKING?)

#include "display/ILI9488/ScreenConnectedDefines480x320.hpp"

// ILI9488 native resolution is 320x480 so we need (for LovyanGFX driver) to invert width/height values & rotate
#define DISPLAY_DRIVER_LOVYANGFX_WIDTH DISPLAY_HEIGHT
#define DISPLAY_DRIVER_LOVYANGFX_HEIGHT DISPLAY_WIDTH
#define DISPLAY_DRIVER_LOVYANGFX_ROTATION 1
#define DISPLAY_DRIVER_LOVYANGFX_ROTATION_MIRROR_FLIP_VERTICAL 3
#define DISPLAY_DRIVER_LOVYANGFX_INVERT_COLORS true

#elif defined(DISPLAY_LOVYANGFX_ST7789_240x320)

#define DISPLAY_PIN_CS 5
#define DISPLAY_PIN_RST 4
#define DISPLAY_PIN_DC 2
#define DISPLAY_PIN_SDA 23
#define DISPLAY_PIN_SCL 18

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_PADDING 1 // real draw starts/ends contained in this horizontal/vertical (pixel) padding (TODO: WORKING?)

#include "display/ST7789/ScreenConnectedDefines240x320.hpp"

// ST7789 native resolution is 240x320 so we need (for LovyanGFX driver) to invert width/height values & rotate
#define DISPLAY_DRIVER_LOVYANGFX_WIDTH DISPLAY_HEIGHT
#define DISPLAY_DRIVER_LOVYANGFX_HEIGHT DISPLAY_WIDTH
#define DISPLAY_DRIVER_LOVYANGFX_ROTATION 3
#define DISPLAY_DRIVER_LOVYANGFX_ROTATION_MIRROR_FLIP_VERTICAL 1
#define DISPLAY_DRIVER_LOVYANGFX_INVERT_COLORS true

#else

#error NO DISPLAY DEFINED

#endif // DISPLAY_LOVYANGFX_ILI9488_480x320

#endif // SDR_REMOTE_CONTROL_DISPLAY_CONFIGURATION_H
