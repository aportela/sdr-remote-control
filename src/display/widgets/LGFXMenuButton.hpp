#ifndef SDR_REMOTE_CONTROL_LGFX_MENU_BUTTON_H
#define SDR_REMOTE_CONTROL_LGFX_MENU_BUTTON_H

#include <LovyanGFX.hpp>
#include "../MenuButton.hpp"

// #define BAR_GROW_LINEAL

class LGFXMenuButton : public MenuButton
{
private:
    LovyanGFX *parentDisplayPtr;

public:
    LGFXMenuButton(LovyanGFX *displayPtr, uint8_t index, uint16_t xOffset, uint16_t yOffset, const char *topLabel, const char *bottomLabel, bool active, ButtonCallback onClick);
    ~LGFXMenuButton();
    void draw(void) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_MENU_BUTTON_H
