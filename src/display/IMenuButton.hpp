#ifndef SDR_REMOTE_CONTROL_MENU_BUTTON_INTERFACE
#define SDR_REMOTE_CONTROL_MENU_BUTTON_INTERFACE

#include <stdint.h>

class IMenuButton
{
public:
    virtual ~IMenuButton() {}
    virtual void draw(uint16_t x, uint16_t y);
};

#endif