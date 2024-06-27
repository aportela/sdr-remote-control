#include "Menu-ILI9498.hpp"

#define DEFAULT_COLOR 0xFFFF
#define DEFAULT_BG 0x0000

// TODO: ADJUST WIDH & MARGIN
#define ROWS 2
#define COLS 4
#define BUTTON_MARGIN_X 12
#define BUTTON_MARGIN_Y 4

#define BUTTON_WIDTH 111
#define BUTTON_HEIGHT 48

// TWO DIGITS TOP LABEL (Fx)
#define TOP_BUTTON_LABEL_X 42
#define TOP_BUTTON_LABEL_Y 5

// MAIN LABEL (Fn DESCRIPTION)
#define MAIN_BUTTON_LABEL_X 9
#define MAIN_BUTTON_LABEL_Y 28

const char *buttonLabels[] = {
    "  TUNE  ",
    " VOLUME ",
    " FILTER ",
    "SETTINGS",
    "VFO  A/B",
    "VFO STEP",
    "VFO MODE",
    ">>>>>>>>",

    "+++ BAND",
    "--- BAND",
    "SETTINGS",
    "        ",
    "        ",
    "        ",
    "        ",
    "<<<<<<<<"};

/*
  "VFO  A/B"
  "VFO STEP"
  "VFO MODE"

  "  TUNE  "
  " VOLUME "
  " FILTER "

  " > BAND "
  " < BAND "

  "SETTINGS"
  "  BACK  "
*/

MenuILI9488::MenuILI9488(LGFX *existingDisplay, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : Menu()
{
    if (existingDisplay != nullptr)
    {
        this->display = existingDisplay;
        this->width = width;
        this->height = height;
        this->xOffset = xOffset;
        this->yOffset = yOffset;
        this->drawMenu();
    }
}

MenuILI9488::~MenuILI9488()
{
}

void MenuILI9488::drawMenu(void)
{
    /*
    "12345678"

    "VFO  A/B"
    "VFO STEP"
    "VFO MODE"

    "  TUNE  "
    " VOLUME "
    " FILTER "

    " > BAND "
    " < BAND "

    "SETTINGS"
    "  BACK  "
    */

    for (uint8_t row = 0, index = 0; row < ROWS; row++)
    {
        for (uint8_t col = 0; col < COLS; col++, index++)
        {
            this->drawButton(index, row, col, buttonLabels[index], index % 2 == 0);
        }
    }
}

void MenuILI9488::drawButton(uint8_t index, uint8_t row, uint8_t col, const char *label, bool active)
{
    uint16_t buttonX = (col * (BUTTON_WIDTH + BUTTON_MARGIN_X)) + this->xOffset;
    uint16_t buttonY = (row * (BUTTON_HEIGHT + BUTTON_MARGIN_Y)) + this->yOffset;
    if (!active)
    {
        this->display->drawRect(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, DEFAULT_COLOR);
    }
    else
    {
        this->display->fillRect(buttonX, buttonY, BUTTON_WIDTH, BUTTON_HEIGHT, DEFAULT_COLOR);
    }

    char buffer[9];
    if (index < 8)
    {
        sprintf(buffer, "F%d", index + 1);
    }
    else
    {
        sprintf(buffer, "F?");
    }
    this->display->setTextSize(2);
    if (!active)
    {
        this->display->setTextColor(DEFAULT_COLOR, DEFAULT_BG);
    }
    else
    {
        this->display->setTextColor(DEFAULT_BG, DEFAULT_COLOR);
    }
    this->display->setCursor(buttonX + TOP_BUTTON_LABEL_X, buttonY + TOP_BUTTON_LABEL_Y);
    this->display->print(buffer);

    if (index < 8)
    {
        sprintf(buffer, buttonLabels[index]);
    }
    else
    {
        sprintf(buffer, "--------");
    }
    this->display->setCursor(buttonX + MAIN_BUTTON_LABEL_X, buttonY + MAIN_BUTTON_LABEL_Y);
    this->display->print(buffer);
}

void MenuILI9488::refresh(uint8_t startMenuIndex, uint8_t selectedMenuIndex)
{
}