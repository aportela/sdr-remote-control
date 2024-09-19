

#ifndef SDR_REMOTE_CONTROL_DISPLAY_CONFIGURATION_H
#define SDR_REMOTE_CONTROL_DISPLAY_CONFIGURATION_H

#define DEBUG_SCREEN_BOUNDS
#define DEBUG_FPS

#define DISPLAY_DRIVER_LOVYANN

#define DISPLAY_LOVYANN_ILI9488_480x320
// #define DISPLAY_LOVYANN_ST7789_240x320

#ifdef DISPLAY_LOVYANN_ILI9488_480x320

#define DISPLAY_PIN_CS 15
#define DISPLAY_PIN_RST 4
#define DISPLAY_PIN_DC 2
#define DISPLAY_PIN_SDA 23
#define DISPLAY_PIN_SCL 18

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

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