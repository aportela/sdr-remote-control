#include "SMeter-ILI9488-Digital.hpp"

#define TOTAL_BARS 42

SMeterILI9488Digital::SMeterILI9488Digital(LGFX *existingDisplay)
{
    if (existingDisplay != nullptr)
    {
        this->display = existingDisplay;
        this->preCalculateBarsSizeAndPosition();
    }
}

SMeterILI9488Digital::~SMeterILI9488Digital()
{
}

void SMeterILI9488Digital::preCalculateBarsSizeAndPosition(void)
{
}

void SMeterILI9488Digital::refresh(uint16_t xOffset, uint16_t yOffset, uint8_t level)
{
}
