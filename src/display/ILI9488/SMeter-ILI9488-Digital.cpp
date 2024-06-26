#include "SMeter-ILI9488-Digital.hpp"

#define COLOR_NOT_ACTIVE 0x18C3
#define COLOR_ACTIVE 0xFFFF

SMeterILI9488Digital::SMeterILI9488Digital(LGFX *existingDisplay, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset)
{
    if (existingDisplay != nullptr)
    {
        this->display = existingDisplay;
        this->width = width;
        this->height = height;
        this->xOffset = xOffset;
        this->yOffset = yOffset;
        this->preCalculateBarsSizeAndPosition();
        this->display->drawRect(this->xOffset, this->yOffset, this->width, this->height, TFT_WHITE);

        this->display->setTextSize(1);
        this->display->setTextColor(COLOR_ACTIVE);
        this->display->setCursor(this->xOffset + 13, this->yOffset + 100);
        this->display->printf("%u", 0);
        this->display->setCursor(this->xOffset + 23, this->yOffset + 100);
        this->display->printf("%u", 1);
        this->display->setCursor(this->xOffset + 33, this->yOffset + 100);
        this->display->printf("%u", 2);
        this->display->setCursor(this->xOffset + 43, this->yOffset + 100);
        this->display->printf("%u", 3);
        this->display->setCursor(this->xOffset + 53, this->yOffset + 100);
        this->display->printf("%u", 4);
        this->display->setCursor(this->xOffset + 63, this->yOffset + 100);
        this->display->printf("%u", 5);
        this->display->setCursor(this->xOffset + 73, this->yOffset + 100);
        this->display->printf("%u", 6);
        this->display->setCursor(this->xOffset + 83, this->yOffset + 100);
        this->display->printf("%u", 7);
        this->display->setCursor(this->xOffset + 93, this->yOffset + 100);
        this->display->printf("%u", 8);
        this->display->setCursor(this->xOffset + 103, this->yOffset + 100);
        this->display->printf("%u", 9);
        this->display->setCursor(this->xOffset + 138, this->yOffset + 100);
        this->display->print("+20");
        this->display->setCursor(this->xOffset + 174, this->yOffset + 100);
        this->display->print("+40");
        this->display->setCursor(this->xOffset + 210, this->yOffset + 100);
        this->display->print("+60");
    }
}

SMeterILI9488Digital::~SMeterILI9488Digital()
{
    this->display = nullptr;
}

void SMeterILI9488Digital::preCalculateBarsSizeAndPosition(void)
{
#ifdef BAR_GROW_LINEAL
    for (uint8_t i = 0; i < BAR_COUNT; i++)
    {
        this->barsX[i] = this->xOffset + 14 + i * (BAR_WIDTH + BAR_HORIZONTAL_MARGIN);
        this->barsY[i] = this->yOffset + 70 - i;
        this->barsHeight[i] = BAR_MIN_HEIGHT + i;
    }
#else
    uint16_t inc = 0;
    for (uint8_t i = 0; i < BAR_COUNT; i++)
    {
        if (i < (BAR_COUNT / 2))
        {
            if (i % 4 == 0)
            {
                inc += 2;
            }
        }
        else
        {
            if (i % 2 == 0)
            {
                inc += 3;
            }
        }
        this->barsX[i] = this->xOffset + 14 + i * (BAR_WIDTH + BAR_HORIZONTAL_MARGIN);
        this->barsY[i] = this->yOffset + 70 - inc;
        this->barsHeight[i] = BAR_MIN_HEIGHT + inc;
    }
#endif
}

void SMeterILI9488Digital::refresh(uint8_t level)
{
    if (level != this->previousSignalMeterLevel)
    {
        this->display->setTextSize(3);
        this->display->setTextColor(COLOR_ACTIVE);
        this->display->setCursor(this->xOffset + 24, this->yOffset + 16);
        // TODO only clear if required (changed from values < S9 to values > 9 or viceversa)
        this->display->fillRect(this->xOffset + 24, this->yOffset + 16, 95, 22, TFT_BLACK);
        //  TODO: check TS2K dB correspondency
        uint16_t decibels = level * 3; // convert SM command to dBs
        if (decibels <= 54)
        {
            this->display->printf("S%udB", decibels / 6);
        }
        else
        {
            this->display->print("S9");
            this->display->setTextSize(2);
            this->display->printf("+%udB", (decibels - 54));
        }

        for (uint8_t i = 0; i < BAR_COUNT; i++)
        {
            uint16_t color = 0;
            if (i < level)
            {
                if (i < 19)
                {
                    color = TFT_GREEN;
                }
                else
                {
                    color = TFT_RED;
                }
            }
            else
            {
                color = COLOR_NOT_ACTIVE;
            }
            this->display->fillRect(this->barsX[i], this->barsY[i], BAR_WIDTH, this->barsHeight[i], color);
        }
    }
    this->previousSignalMeterLevel = level;
}
