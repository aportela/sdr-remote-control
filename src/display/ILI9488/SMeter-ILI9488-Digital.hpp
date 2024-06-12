#ifndef SDR_REMOTE_CONTROL_SMETER_ILI9488_DIGITAL_H
#define SDR_REMOTE_CONTROL_SMETER_ILI9488_DIGITAL_H

#include <stdio.h>
#include "LGFX.hpp"
#include "../SMeter.hpp"

#define BAR_COUNT 42
#define BAR_WIDTH 3
#define BAR_MIN_HEIGHT 18
#define BAR_HORIZONTAL_MARGIN 2

// #define BAR_GROW_LINEAL

class SMeterILI9488Digital : public SMeter
{
public:
    SMeterILI9488Digital(LGFX *existingDisplay, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~SMeterILI9488Digital();
    void refresh(uint8_t level) override;

private:
    LGFX *display;
    void preCalculateBarsSizeAndPosition(void);
    uint16_t barsX[BAR_COUNT];
    uint16_t barsY[BAR_COUNT];
    uint16_t barsHeight[BAR_COUNT];
    uint8_t previousSignalMeterLevel = 43;
};

#endif
