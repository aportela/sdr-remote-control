#include "LGFXAnalogSMeterWidget.hpp"

#define ANALOG_SMETER_BACKGROUND_COLOR 0xF75B // 0xE6F9
#define ANALOG_SMETER_TEXT_COLOR TEXT_COLOR_NOT_ACTIVE
#define ANALOG_SMETER_INDICATOR_COLOR 0xF388

LGFXAnalogSMeterWidget::LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXSMeterWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
    if (displayPtr != nullptr && currentTransceiverStatusPtr != nullptr)
    {
        this->refresh(true);
    }
}

LGFXAnalogSMeterWidget::~LGFXAnalogSMeterWidget()
{
}

#ifdef USE_HORIZONTAL_GRADIENT_ON_ANALOG_SMETER_WIDGET

uint32_t LGFXAnalogSMeterWidget::getGradientColor(int index)
{
    uint32_t TFT_GREEN = 0x00FF00; // RGB: (0, 255, 0)
    uint32_t ORANGE = 0xFFA500;    // RGB: (255, 165, 0)
    uint32_t TFT_RED = 0xFF0000;   // RGB: (255, 0, 0)

    int greenR = (TFT_GREEN >> 16) & 0xFF;
    int greenG = (TFT_GREEN >> 8) & 0xFF;
    int greenB = TFT_GREEN & 0xFF;

    int orange_R = (ORANGE >> 16) & 0xFF;
    int orange_G = (ORANGE >> 8) & 0xFF;
    int orange_B = ORANGE & 0xFF;

    int redR = (TFT_RED >> 16) & 0xFF;
    int redG = (TFT_RED >> 8) & 0xFF;
    int redB = TFT_RED & 0xFF;

    const int num_steps = 342;

    if (index < 0 || index >= num_steps)
    {
        return 0;
    }

    uint32_t color;

    if (index <= 120)
    {
        int R = greenR + ((orange_R - greenR) * index) / 120;
        int G = greenG + ((orange_G - greenG) * index) / 120;
        int B = greenB + ((orange_B - greenB) * index) / 120;

        color = (R << 16) | (G << 8) | B;
    }
    else
    {
        int i = index - 120;

        int R = orange_R + ((redR - orange_R) * i) / 222;
        int G = orange_G + ((redG - orange_G) * i) / 222;
        int B = orange_B + ((redB - orange_B) * i) / 222;

        color = (R << 16) | (G << 8) | B;
    }

    return (color);
}

#endif

void LGFXAnalogSMeterWidget::init(void)
{
    this->parentDisplayPtr->fillRoundRect(this->xOffset + this->padding, this->yOffset + this->padding, _ANALOG_SMETER_WIDGET_BACKGROUND_WIDTH, _ANALOG_SMETER_WIDGET_BACKGROUND_HEIGHT, 4, ANALOG_SMETER_BACKGROUND_COLOR);
    this->parentDisplayPtr->drawRoundRect(this->xOffset + this->padding, this->yOffset + this->padding, _ANALOG_SMETER_WIDGET_BACKGROUND_WIDTH, _ANALOG_SMETER_WIDGET_BACKGROUND_HEIGHT, 4, TEXT_COLOR_SECONDARY);
    this->parentDisplayPtr->setTextSize(2);
    this->parentDisplayPtr->setTextColor(ANALOG_SMETER_TEXT_COLOR);
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_NUMBERS_X_OFFSET, this->yOffset + this->padding + 2);
    this->parentDisplayPtr->print("1  3  5  7  9 +15 +30    +60");

#ifdef USE_HORIZONTAL_GRADIENT_ON_ANALOG_SMETER_WIDGET
    for (uint16_t i = 0; i < _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH; i++)
    {
        this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET + i, this->yOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 5, 1, 5, getGradientColor(i));
    }
#else
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, this->yOffset + this->padding + 7 + _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 5, 155, 5, TFT_GREEN);
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + 159, this->yOffset + this->padding + 7 + _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 5, _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH - 155, 5, TFT_RED);
#endif
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, this->yOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH, 2, ANALOG_SMETER_TEXT_COLOR);

    for (int i = 1; i < _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
    {
#define B_WIDTH 3
#define B_MARGIN 6
        uint16_t x = this->xOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET + (i * (B_WIDTH + B_MARGIN));
        bool isCurrentLongBar = (i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 22 || i == 27 || i == 32 || i == 37);
        this->parentDisplayPtr->fillRect(x, this->yOffset + this->padding + (isCurrentLongBar ? _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 10 : _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 3), B_WIDTH, isCurrentLongBar ? 10 : 3, ANALOG_SMETER_TEXT_COLOR);
    }
}

void LGFXAnalogSMeterWidget::update(int8_t dB)
{
    uint16_t xof = map(this->previousDBSmoothedValue, SMETER_MIN_DB, SMETER_MAX_DB, this->xOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH) - 5;
    // clear previous value
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + xof, this->yOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, ANALOG_SMETER_BACKGROUND_COLOR);
    // draw current value
    xof = map(dB, SMETER_MIN_DB, SMETER_MAX_DB, this->xOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH) - 5;
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + xof, this->yOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, ANALOG_SMETER_INDICATOR_COLOR);
}