#ifndef SDR_REMOTE_CONTROL_MENU_BUTTON_INTERFACE
#define SDR_REMOTE_CONTROL_MENU_BUTTON_INTERFACE

#include <stdint.h>

class IMenuButton
{
public:
    virtual ~IMenuButton() {}
    virtual void draw(void) = 0;
    virtual void onChange(void) = 0;
    virtual void setActive(bool flag) = 0;
    virtual void click(void) = 0;
};

#endif