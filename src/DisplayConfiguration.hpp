

#ifndef SDR_REMOTE_CONTROL_DISPLAY_CONFIGURATION_H
#define SDR_REMOTE_CONTROL_DISPLAY_CONFIGURATION_H

#define DEBUG_SCREEN_WIDGETS_BOUNDS_BORDER_COLOR TFT_WHITE
// #define DEBUG_SCREEN_BOUNDS
#define DEBUG_SCREEN_WIDGETS_BOUNDS
#define DEBUG_FPS

#define DISPLAY_DRIVER_LOVYANN

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

// #define DISPLAY_LOVYANN_ILI9488_480x320
#define DISPLAY_LOVYANN_ST7789_240x320

#ifdef DISPLAY_LOVYANN_ILI9488_480x320

#define DISPLAY_PIN_CS 15
#define DISPLAY_PIN_RST 4
#define DISPLAY_PIN_DC 2
#define DISPLAY_PIN_SDA 23
#define DISPLAY_PIN_SCL 18

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320
#define DISPLAY_PADDING 1 // real draw starts/ends contained in this horizontal/vertical (pixel) padding

#define DEFAULT_TEXT_COLOR TFT_WHITE
#define DEFAULT_TEXT_BACKGROUND_COLOR TFT_BLACK
#define TEXT_COLOR_ACTIVE DEFAULT_TEXT_COLOR
#define TEXT_COLOR_SECONDARY 0x528A
#define TEXT_COLOR_NOT_ACTIVE 0x18C3
#define TEXT_BACKGROUND_COLOR DEFAULT_TEXT_BACKGROUND_COLOR // TODO: remove ?

// TODO: refactor to ScreenConnectedDefines480x320.hpp
#include "display/ILI9488/ScreenConnectedDefines.hpp"

// ILI9488 native resolution is 320x480 so we need (for LovyAnn driver) to invert width/height values & rotate
#define DISPLAY_DRIVER_LOVYANN_WIDTH DISPLAY_HEIGHT
#define DISPLAY_DRIVER_LOVYANN_HEIGHT DISPLAY_WIDTH
#define DISPLAY_DRIVER_LOVYANN_ROTATION 1
#define DISPLAY_DRIVER_LOVYANN_ROTATION_MIRROR_FLIP_VERTICAL 3
#define DISPLAY_DRIVER_LOVYANN_INVERT_COLORS true

#elif defined(DISPLAY_LOVYANN_ST7789_240x320)

#define DISPLAY_PIN_CS 5
#define DISPLAY_PIN_RST 4
#define DISPLAY_PIN_DC 2
#define DISPLAY_PIN_SDA 23
#define DISPLAY_PIN_SCL 18

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_PADDING 1 // real draw starts/ends contained in this horizontal/vertical (pixel) padding

#define DEFAULT_TEXT_COLOR TFT_WHITE
#define DEFAULT_TEXT_BACKGROUND_COLOR TFT_BLACK
#define TEXT_COLOR_ACTIVE DEFAULT_TEXT_COLOR
#define TEXT_COLOR_SECONDARY 0x528A
#define TEXT_COLOR_NOT_ACTIVE 0x18C3
#define TEXT_BACKGROUND_COLOR DEFAULT_TEXT_BACKGROUND_COLOR // TODO: remove ?

#include "display/ST7789/ScreenConnectedDefines.hpp"

// ST7789 native resolution is 240x320 so we need (for LovyAnn driver) to invert width/height values & rotate
#define DISPLAY_DRIVER_LOVYANN_WIDTH DISPLAY_HEIGHT
#define DISPLAY_DRIVER_LOVYANN_HEIGHT DISPLAY_WIDTH
#define DISPLAY_DRIVER_LOVYANN_ROTATION 3
#define DISPLAY_DRIVER_LOVYANN_ROTATION_MIRROR_FLIP_VERTICAL 1
#define DISPLAY_DRIVER_LOVYANN_INVERT_COLORS true

#else

#error NO DISPLAY DEFINED

#endif // DISPLAY_LOVYANN_ILI9488_480x320

#endif // SDR_REMOTE_CONTROL_DISPLAY_CONFIGURATION_H
