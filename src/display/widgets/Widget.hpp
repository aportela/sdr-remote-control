#ifndef SDR_REMOTE_CONTROL_WIDGET_H
#define SDR_REMOTE_CONTROL_WIDGET_H

#include <cstdint>

#include "../../Transceiver.hpp"

class Widget
{
protected:
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t xOffset = 0;
    uint16_t yOffset = 0;
    uint8_t padding = 0;

public:
    Widget(uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding = 0);
    virtual ~Widget() {}
    // virtual bool refresh(bool force = false) = 0;
};

#endif // SDR_REMOTE_CONTROL_WIDGET_H
