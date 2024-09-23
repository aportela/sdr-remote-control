#include "MenuButton.hpp"

MenuButton::MenuButton(uint8_t index, uint16_t xOffset, uint16_t yOffset, uint16_t width, uint16_t height, const char *topLabel, const char *bottomLabel, bool active, ButtonCallback onClick)
    : index(index), xOffset(xOffset), yOffset(yOffset), width(width), height(height), active(active), onClick(onClick), changed(true)
{
    snprintf(this->topLabel, sizeof(this->topLabel), "%s", strlen(topLabel) == MENU_BUTTON_TOP_LABEL_LENGTH ? topLabel : "LE");
    snprintf(this->bottomLabel, sizeof(this->bottomLabel), "%s", strlen(bottomLabel) == MENU_BUTTON_BOTTOM_LABEL_LENGTH ? bottomLabel : "LEN  ERR");
}

MenuButton::~MenuButton()
{
}

void MenuButton::onChange(void)
{
    this->changed = true;
}

void MenuButton::setActive(bool flag)
{
    this->active = flag;
    this->onChange();
}

void MenuButton::click(void)
{
    if (this->onClick)
    {
        this->onClick();
    }
}
