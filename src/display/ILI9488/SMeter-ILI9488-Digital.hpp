#ifndef SDR_REMOTE_CONTROL_SMETER_ILI9488_DIGITAL_H
#define SDR_REMOTE_CONTROL_SMETER_ILI9488_DIGITAL_H

#include <stdio.h>
#include "LGFX.hpp"
#include "../SMeter.hpp"

class SMeterILI9488Digital : public SMeter
{
public:
    SMeterILI9488Digital(LGFX *existingDisplay);
    ~SMeterILI9488Digital();
    void refresh(uint16_t xOffset, uint16_t yOffset, uint8_t level) override;

private:
    LGFX *display;
    void preCalculateBarsSizeAndPosition(void);
};

#endif
