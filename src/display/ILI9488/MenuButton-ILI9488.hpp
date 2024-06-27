#ifndef SDR_REMOTE_CONTROL_MENU_BUTTON_ILI9488_DIGITAL_H
#define SDR_REMOTE_CONTROL_MENU_BUTTON_ILI9488_DIGITAL_H

#include <stdio.h>
#include "LGFX.hpp"
#include "../MenuButton.hpp"

// #define BAR_GROW_LINEAL

class MenuButtonILI9488 : public MenuButton
{
public:
    MenuButtonILI9488(LGFX *existingDisplay, uint8_t index, char *topLabel, char *bottomLabel, bool active, ButtonCallback onClick);
    ~MenuButtonILI9488();
    void draw(uint16_t x, uint16_t y) override;

private:
    LGFX *display;
};

#endif
