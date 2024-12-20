#ifndef SDR_REMOTE_CONTROL_LGFX_MENU_BUTTON_H
#define SDR_REMOTE_CONTROL_LGFX_MENU_BUTTON_H

#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"

#define MENU_BUTTON_TOP_LABEL_LENGTH 2
#define MENU_BUTTON_BOTTOM_LABEL_LENGTH 8

typedef void (*ButtonCallback)();

class LGFXMenuButton : public LGFXWidget
{
private:
    uint8_t index;
    char topLabel[MENU_BUTTON_TOP_LABEL_LENGTH + 1];
    char bottomLabel[MENU_BUTTON_BOTTOM_LABEL_LENGTH + 1];
    bool active = false;
    ButtonCallback onClick;
    bool changed = true;
    bool topLabelVisible = true;
    bool bottomLabelVisible = true;

public:
    LGFXMenuButton(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, uint8_t index, const char *topLabel, bool topLabelVisible, const char *bottomLabel, bool bottomLabelVisible, bool active, ButtonCallback onClick);
    ~LGFXMenuButton();
    bool refresh(bool force = false) override;
    void onChange(void);
    bool isActive(void);
    void setActive(bool flag);
    void click(void);
};

#endif // SDR_REMOTE_CONTROL_LGFX_MENU_BUTTON_H
