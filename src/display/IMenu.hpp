#ifndef SDR_REMOTE_CONTROL_MENU_INTERFACE
#define SDR_REMOTE_CONTROL_MENU_INTERFACE

#include <stdint.h>

class IMenu
{
public:
    virtual ~IMenu() {}
    virtual void refresh(uint8_t startMenuIndex, uint8_t selectedMenuIndex) = 0;
    virtual void previousPage(void) = 0;
    virtual void nextPage(void) = 0;
};

#endif