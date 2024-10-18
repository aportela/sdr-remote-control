#ifndef SDR_REMOTE_CONTROL_MENU_ITEM_H
#define SDR_REMOTE_CONTROL_MENU_ITEM_H

#include <cstdint>
#include <cstddef>
#include "MenuConfiguration.hpp"

#define MENU_ITEM_LABEL_LENGTH 8

class MenuItem
{
private:
    char label[MENU_ITEM_LABEL_LENGTH + 1] = {'\0'};
    bool active = false;
    bool blink = false;
    uint64_t blinkStartTimer = 0;

public:
    MenuItem(const char *label);
    ~MenuItem();
    void getLabel(char *buf, size_t count);
    void setActive(bool flag);
    void click(void);
    bool isActive(void);
    bool isBlinking(void);
};

#endif // SDR_REMOTE_CONTROL_MENU_ITEM_H
