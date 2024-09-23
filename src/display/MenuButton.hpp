#ifndef SDR_REMOTE_CONTROL_MENU_BUTTON_H
#define SDR_REMOTE_CONTROL_MENU_BUTTON_H

#include <Arduino.h>

#include "IMenuButton.hpp"

#define MENU_BUTTON_TOP_LABEL_LENGTH 2
#define MENU_BUTTON_BOTTOM_LABEL_LENGTH 8

typedef void (*ButtonCallback)();

class MenuButton : public IMenuButton
{
protected:
    uint8_t index;
    uint16_t xOffset = 0;
    uint16_t yOffset = 0;
    uint16_t width;
    uint16_t height;
    char topLabel[MENU_BUTTON_TOP_LABEL_LENGTH + 1];
    char bottomLabel[MENU_BUTTON_BOTTOM_LABEL_LENGTH + 1];
    bool active;
    ButtonCallback onClick;
    bool changed;

public:
    MenuButton(uint8_t index, uint16_t xOffset, uint16_t yOffset, uint16_t width, uint16_t height, const char *topLabel, const char *bottomLabel, bool active, ButtonCallback onClick);
    ~MenuButton();
    virtual void draw(void) = 0;
    void onChange(void) override;
    void setActive(bool flag) override;
    void click(void) override;
};

#endif
