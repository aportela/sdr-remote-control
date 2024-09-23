#include "MenuButton-ILI9488.hpp"

#define BUTTON_WIDTH 111
#define BUTTON_HEIGHT 46

#define BUTTON_NOT_ACTIVE_BACKGROUND_COLOR 0x0000
#define BUTTON_ACTIVE_BACKGROUND_COLOR 0xFFFF

#define BUTTON_NOT_ACTIVE_LABEL_COLOR 0xFFFF
#define BUTTON_ACTIVE_LABEL_COLOR 0x0000

MenuButtonILI9488::MenuButtonILI9488(LGFX *existingDisplay, uint8_t index, uint16_t x, uint16_t y, const char *topLabel, const char *bottomLabel, bool active, ButtonCallback onClick)
    : MenuButton(index, x, y, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char *>(topLabel), const_cast<char *>(bottomLabel), active, onClick)
{
    if (existingDisplay != nullptr)
    {
        this->display = existingDisplay;
    }
}

MenuButtonILI9488::~MenuButtonILI9488()
{
    this->display = nullptr;
}

void MenuButtonILI9488::draw(void)
{
    if (this->changed)
    {
        if (!this->active)
        {
            this->display->fillRect(this->x, this->y, this->width, this->height, BUTTON_NOT_ACTIVE_BACKGROUND_COLOR);
            this->display->drawRect(this->x, this->y, this->width, this->height, BUTTON_ACTIVE_BACKGROUND_COLOR);
        }
        else
        {
            this->display->fillRect(this->x, this->y, this->width, this->height, BUTTON_ACTIVE_BACKGROUND_COLOR);
        }
        this->display->setTextSize(2);
        if (!active)
        {
            this->display->setTextColor(BUTTON_NOT_ACTIVE_LABEL_COLOR, BUTTON_NOT_ACTIVE_BACKGROUND_COLOR);
        }
        else
        {
            this->display->setTextColor(BUTTON_ACTIVE_LABEL_COLOR, BUTTON_ACTIVE_BACKGROUND_COLOR);
        }
        this->display->setCursor(this->x + 44, this->y + 5);
        this->display->print(this->topLabel);
        this->display->setCursor(this->x + 8, this->y + 26);
        this->display->print(this->bottomLabel);
        this->changed = false;
    }
}