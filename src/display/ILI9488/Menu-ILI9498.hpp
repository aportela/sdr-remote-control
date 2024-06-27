#ifndef SDR_REMOTE_CONTROL_MENU_ILI9488_DIGITAL_H
#define SDR_REMOTE_CONTROL_MENU_ILI9488_DIGITAL_H

#include <stdio.h>
#include "LGFX.hpp"
#include "../Menu.hpp"
#include <string.h>

class MenuILI9488 : public Menu
{
public:
    MenuILI9488(LGFX *existingDisplay, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~MenuILI9488();
    void refresh(uint8_t startMenuIndex, uint8_t selectedMenuIndex) override;

private:
    LGFX *display;

    void drawMenu(void);
    void drawButton(uint8_t index, uint8_t row, uint8_t col, const char *label, bool active);
};

#endif
