#ifndef SDR_REMOTE_CONTROL_MENU_H
#define SDR_REMOTE_CONTROL_MENU_H

#include <Arduino.h>
#include "IMenu.hpp"

class Menu : public IMenu
{
public:
    Menu(void);
    virtual void refresh(uint8_t startMenuIndex, uint8_t selectedMenuIndex) = 0;
    virtual void previousPage(void) = 0;
    virtual void nextPage(void) = 0;

private:
protected:
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t xOffset = 0;
    uint16_t yOffset = 0;
    uint8_t currentPage = 0;
    uint8_t totalPages = 0;
};
#endif
