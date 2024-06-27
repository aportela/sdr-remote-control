#ifndef SDR_REMOTE_CONTROL_MENU_ILI9488_DIGITAL_H
#define SDR_REMOTE_CONTROL_MENU_ILI9488_DIGITAL_H

#include <stdio.h>
#include "LGFX.hpp"
#include "../Menu.hpp"
#include "MenuButton-ILI9488.hpp"

#include <string.h>

#define TOTAL_MENU_BUTTONS 16

class MenuILI9488 : public Menu
{
public:
    MenuILI9488(LGFX *existingDisplay, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~MenuILI9488();
    void refresh(bool forceAll) override;
    void previousPage(void) override;
    void nextPage(void) override;

private:
    LGFX *display;
    MenuButtonILI9488 *buttons[TOTAL_MENU_BUTTONS];
    void initMenu(void);
};

#endif
