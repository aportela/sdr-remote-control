#ifndef SDR_REMOTE_CONTROL_LGFX_MENU_H
#define SDR_REMOTE_CONTROL_LGFX_MENU_H

#include <stdio.h>
#include <LovyanGFX.hpp>

#include "../Menu.hpp"
#include "LGFXMenuButton.hpp"

#include <string.h>

#define TOTAL_MENU_BUTTONS 16

class LGFXMenu : public Menu
{
private:
    LovyanGFX *parentDisplayPtr;
    LGFXMenuButton *buttons[TOTAL_MENU_BUTTONS];
    void initMenu(void);

public:
    LGFXMenu(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    ~LGFXMenu();
    void refresh(bool forceAll) override;
    void setButtonEnabled(uint8_t btnIndex);
    void setButtonDisabled(uint8_t btnIndex);
    void previousPage(void) override;
    void nextPage(void) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_MENU_H
