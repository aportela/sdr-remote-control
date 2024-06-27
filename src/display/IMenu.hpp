#ifndef SDR_REMOTE_CONTROL_MENU_INTERFACE
#define SDR_REMOTE_CONTROL_MENU_INTERFACE

#include <stdint.h>

class IMenu
{
public:
    virtual ~IMenu() {}
    virtual void refresh(uint8_t startMenuIndex, uint8_t selectedMenuIndex);
    virtual void previousPage(void);
    virtual void nextPage(void);
};

#endif