#ifndef SDR_REMOTE_CONTROL_MENU_BUTTON_H
#define SDR_REMOTE_CONTROL_MENU_BUTTON_H

#include <Arduino.h>

#include "IMenuButton.hpp"

typedef void (*ButtonCallback)();

class MenuButton : public IMenuButton
{
protected:
    uint8_t index;
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t width;
    uint16_t height;
    char *topLabel;
    char *bottomLabel;
    bool active;
    ButtonCallback onClick;
    bool changed;

public:
    MenuButton(uint8_t index, uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *topLabel, char *bottomLabel, bool active, ButtonCallback onClick);
    ~MenuButton();
    virtual void draw(void) = 0;
    void onChange();
    void setActive(bool flag);
    void click(void);
};

#endif
