#include "MenuButton.hpp"

MenuButton::MenuButton(uint8_t index, uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *topLabel, char *bottomLabel, bool active, ButtonCallback onClick)
    : index(index), x(x), y(y), width(width), height(height), topLabel(topLabel), bottomLabel(bottomLabel), active(active), onClick(onClick), changed(true)
{
}

void MenuButton::onChange()
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
