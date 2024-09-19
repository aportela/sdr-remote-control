#ifndef SDR_REMOTE_CONTROL_WIDGET_H
#define SDR_REMOTE_CONTROL_WIDGET_H

#include <cstdint>

class Widget
{
protected:
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t xOffset = 0;
    uint16_t yOffset = 0;

public:
    Widget(uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset);
    virtual ~Widget() {}
    virtual void refresh(void) = 0;
};

#endif // SDR_REMOTE_CONTROL_WIDGET_H
