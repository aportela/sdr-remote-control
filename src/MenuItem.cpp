#include <Arduino.h>
#include "MenuItem.hpp"

#define MENU_ITEM_BLINK_TIMER 100 // 100ms

MenuItem::MenuItem(const char *label)
{
    strncpy(this->label, label, MENU_ITEM_LABEL_LENGTH);
    this->label[MENU_ITEM_LABEL_LENGTH] = '\0';
}

MenuItem::~MenuItem()
{
}

void MenuItem::getLabel(char *buf, size_t count)
{
    if (count > 0)
    {
        strncpy(buf, this->label, count - 1);
        buf[count - 1] = '\0';
    }
}

void MenuItem::setActive(bool flag)
{
    this->blink = false;
    this->active = flag;
}

void MenuItem::click(void)
{
    this->blinkStartTimer = millis();
    this->blink = true;
}

bool MenuItem::isActive(void)
{
    return (this->active);
}

bool MenuItem::isBlinking(void)
{
    if (this->blink && millis() - this->blinkStartTimer < MENU_ITEM_BLINK_TIMER)
    {
        return (true);
    }
    else
    {
        this->blink = false;
        return (false)
    }
}
