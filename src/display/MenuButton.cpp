#include "MenuButton.hpp"

#define TOP_LABEL_LENGTH 2
#define BOTTOM_LABEL_LENGTH 8

MenuButton::MenuButton(uint8_t index, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *topLabel, const char *bottomLabel, bool active, ButtonCallback onClick)
    : index(index), x(x), y(y), width(width), height(height), active(active), onClick(onClick), changed(true)
{
    this->topLabel = new char[TOP_LABEL_LENGTH + 1];
    if (strlen(topLabel) == TOP_LABEL_LENGTH)
    {
        strncpy(this->topLabel, topLabel, TOP_LABEL_LENGTH);
        this->topLabel[TOP_LABEL_LENGTH] = '\0';
    }
    else
    {
        sprintf(this->topLabel, "LE"); // top label length error
    }

    this->bottomLabel = new char[BOTTOM_LABEL_LENGTH + 1];
    if (strlen(bottomLabel) == BOTTOM_LABEL_LENGTH)
    {
        strncpy(this->bottomLabel, bottomLabel, BOTTOM_LABEL_LENGTH);
        this->bottomLabel[BOTTOM_LABEL_LENGTH] = '\0';
    }
    else
    {
        sprintf(this->bottomLabel, "LEN  ERR"); // bottom label length error
    }
}

MenuButton::~MenuButton()
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
